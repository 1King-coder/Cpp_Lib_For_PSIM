#include "sci_com.h"
#include "global_definitions.h"
#include "F28x_Project.h"


void setup_baud (float baudRate) {
    uint32_t lspclk_hz;
    
    // 1. Read the actual LOSPCP hardware divider 
    uint16_t lospcp_val = ClkCfgRegs.LOSPCP.bit.LSPCLKDIV; 

    // 2. Calculate LSPCLK based on the TRM divider logic 
    if (lospcp_val == 0) {
        lspclk_hz = DSP_CLOCK;         // /1 divider
    } else {
        lspclk_hz = DSP_CLOCK / (lospcp_val * 2); // /2, /4, /6, /8, /10, /12, /14 dividers
    }

    // 3. Calculate exact BRR value with standard +0.5 rounding [cite: 499]
    float brr_float = ((float)lspclk_hz / (baudRate * 8.0f)) - 1.0f + 0.5f;
    uint16_t brr_val = (uint16_t)brr_float;

    // 4. Split and apply to the High and Low Baud Registers 
    SciaRegs.SCIHBAUD.all = (brr_val >> 8) & 0xFF;
    SciaRegs.SCILBAUD.all = brr_val & 0xFF;
}

void scia_com_init()
{

    SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    setup_baud(SCI_TARGET_BAUD);

    SciaRegs.SCICTL1.all = 0x0023;
}

void scia_fifo_init(int receivedDataSize)
{

    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 0;
    SciaRegs.SCIFFTX.bit.TXFIFORESET = 1;
    
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0; 
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1; 
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;    
    SciaRegs.SCIFFRX.bit.RXFFIL = receivedDataSize; // NÍVEL DE INTERRUPÇÃO

    SciaRegs.SCIFFCT.all = 0x0;
}

void scia_init (void) {
    scia_com_init();
    scia_fifo_init(RECEIVED_DATA_SIZE);
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
