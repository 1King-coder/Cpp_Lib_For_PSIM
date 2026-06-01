
//
// Included Files
//
#include "F28x_Project.h"

//
// Globals
//
Uint16 LoopCount;

//
// Function Prototypes
//
void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);
void startup(void) {
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xD_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the F2837xD_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    InitGpio();

    //
    // For this example, only init the pins for the SCI-A port.
    //  GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    //  GPIO_SetupPinOptions() - Sets the direction and configuration of the GPIOS
    // These functions are found in the F2837xD_Gpio.c file.
    //
#ifdef _LAUNCHXL_F2837xD
    GPIO_SetupPinMux(43, GPIO_MUX_CPU1, 15); 
    GPIO_SetupPinOptions(43, GPIO_INPUT, GPIO_PUSHPULL);
    
    GPIO_SetupPinMux(42, GPIO_MUX_CPU1, 15); 
    GPIO_SetupPinOptions(42, GPIO_OUTPUT, GPIO_ASYNC);
#else
    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);
#endif

    //
    // Step 3. Clear all __interrupts and initialize PIE vector table:
    // Disable CPU __interrupts
    //
    DINT;

    //
    // Initialize PIE control registers to their default state.
    // The default state is all PIE __interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU __interrupts and clear all CPU __interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the __interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    //
    InitPieVectTable();
}
//
// Main
//

typedef struct  {
    float kp;
    float ki;
} PI_GAINS;

float u1 = 0;
float y1 = 0;

void PI_c (float* out, float* u, PI_GAINS gains, float timestep) {
    *out = 2*gains.kp * (*u - u1) + gains.ki * timestep * (*u + u1) + y1;
    u1 = *u;
    y1 = *out;
}

struct SimData {
    float u;
    float y;
    PI_GAINS gains;
};

void main(void)
{
    struct SimData data;
    float y;

    
    startup();
    //
    // Step 4. User specific code:
    //
    LoopCount = 0;

    scia_fifo_init();       // Initialize the SCI FIFO
    scia_echoback_init();   // Initialize SCI for echoback

    for(;;)
    {
        int i = 0;

        for (i = 0; i < 16; i++) {
            while(SciaRegs.SCIFFRX.bit.RXFFST == 0) { }
            __byte((char*) &data, i) = SciaRegs.SCIRXBUF.all;
        }

        PI_c(&y, &data.u, data.gains, 1e-6);

        for (i = 0; i<4; i++) {
            while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
            SciaRegs.SCITXBUF.all = __byte((char*) &y, i);
        }
        LoopCount++;
    }
}

//
//  scia_echoback_init - Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F,
//                       default, 1 STOP bit, no parity
//
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

    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
    float LSPCLK_FREQ = 25000000.0; // ALTERE PARA 25000000.0 SE O LIXO CONTINUAR
    float SCI_BAUD = 9600.0;        // Certifique-se que o Terminal/Putty está em 9600
    
    // Cálculo exato do Baud Rate Register (BRR)
    Uint16 brr_val = (Uint16)((LSPCLK_FREQ / (SCI_BAUD * 8.0)) - 1.0 + 0.5);

    SciaRegs.SCIHBAUD.all = (brr_val >> 8) & 0xFF;
    SciaRegs.SCILBAUD.all = brr_val & 0xFF;

    SciaRegs.SCICTL1.all = 0x0023;
}

//
// scia_xmit - Transmit a character from the SCI
//
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST == 16) {} 
    SciaRegs.SCITXBUF.all = a;
}

//
// scia_msg - Transmit message via SCIA
//
void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}

//
// scia_fifo_init - Initialize the SCI FIFO
//
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;
}

//
// End of file
//
