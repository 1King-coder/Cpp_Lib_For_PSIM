
//
// Included Files
//
#include "F28x_Project.h"

//
// Globals
//
Uint16 LoopCount;
#define BAUD 115200.0
//
// Function Prototypes
//
void scia_echoback_init(void);
void scia_fifo_init(void);
void receiveData (void *buff, int size);
void sendData(void *buff, int size);

void startup(void) {

    InitSysCtrl();

    InitGpio();

    GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15); 
    GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_PUSHPULL);
    
    GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15); 
    GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_ASYNC);

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();
}

// global variables
typedef struct  {
    float kp;
    float ki;
} PI_GAINS;

float u1 = 0;
float y1 = 0;

struct SimData {
    float u;
    float y;
    PI_GAINS gains;
};

struct SimData data;

float y;
int dataReceived = 0;

void PI_c (float* out, float* u, PI_GAINS gains, float sampling_time) {
    *out = gains.kp * (*u - u1) + gains.ki * sampling_time * (*u + u1) / 2 + y1;
    if (*out >= 8) {
        *out = 8;
    } else if (*out <=0) {
        *out = 0;
    }
    u1 = *u;
    y1 = *out;
}



interrupt void processDataReceiveINT (void) {
    receiveData(&data, 16);
    PI_c(&y, &data.u, data.gains, 1/40e3);

    sendData(&y, 4);

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
    
}


void main(void)
{
    
    startup();

    LoopCount = 0;

    EALLOW;
    PieVectTable.SCIA_RX_INT = &processDataReceiveINT;
    EDIS;
    
    scia_fifo_init();       
    scia_echoback_init();

    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    IER |= M_INT9;
    EINT;

    for(;;)
    {

        // if (dataReceived) {
        //     sendData(&y, 4);
        //     dataReceived = 0;
        // }

        LoopCount++;
    }
}

void setup_baud (float baudRate) {
    uint32_t lspclk_hz;
    
    // 1. Read the actual LOSPCP hardware divider 
    uint16_t lospcp_val = ClkCfgRegs.LOSPCP.bit.LSPCLKDIV; 

    // 2. Calculate LSPCLK based on the TRM divider logic 
    if (lospcp_val == 0) {
        lspclk_hz = 200e6;         // /1 divider
    } else {
        lspclk_hz = 200e6 / (lospcp_val * 2); // /2, /4, /6, /8, /10, /12, /14 dividers
    }

    // 3. Calculate exact BRR value with standard +0.5 rounding [cite: 499]
    float brr_float = ((float)lspclk_hz / (baudRate * 8.0f)) - 1.0f + 0.5f;
    uint16_t brr_val = (uint16_t)brr_float;

    // 4. Split and apply to the High and Low Baud Registers 
    SciaRegs.SCIHBAUD.all = (brr_val >> 8) & 0xFF;
    SciaRegs.SCILBAUD.all = brr_val & 0xFF;
}
void scia_echoback_init()
{
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //

    SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

   setup_baud(BAUD);

    SciaRegs.SCICTL1.all = 0x0023;
}

void sendData(void *buff, int size)
{
    int i;
    for (i = 0; i<size; i++) {
        while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
        SciaRegs.SCITXBUF.all = __byte((int*) buff, i);
    }
}

void receiveData (void *buff, int size)
{
    int i;
    for (i = 0; i < size; i++) {

        __byte((int*) buff, i) = SciaRegs.SCIRXBUF.all;
    }
}

//
// scia_fifo_init - Initialize the SCI FIFO
//
void scia_fifo_init()
{

    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 0;
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;
    
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0; 
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1; 
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;    
    SciaRegs.SCIFFRX.bit.RXFFIL = 16;      // NÍVEL DE INTERRUPÇÃO: Dispara a ISR quando chegar a 4 bytes (Tamanho de 1 float)

    SciaRegs.SCIFFCT.all = 0x0;
}

//
// End of file
//
