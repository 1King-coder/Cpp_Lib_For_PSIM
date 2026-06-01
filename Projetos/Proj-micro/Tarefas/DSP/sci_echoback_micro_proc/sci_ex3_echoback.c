
//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
// Define AUTOBAUD to use the autobaud lock feature
//#define AUTOBAUD

//
// Globals
//
uint16_t loopCounter = 0;
#define BAUD 9600

void startup (void) {
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Configuration for the SCI Rx pin.
    //
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    //
    // Configuration for the SCI Tx pin.
    //
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    //
    // Initialize interrupt controller and vector table.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //
    // Initialize SCIA and its FIFO.
    //
    SCI_performSoftwareReset(SCIA_BASE);

    //
    // Configure SCIA for echoback.
    //
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, BAUD, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

#ifdef AUTOBAUD
    //
    // Perform an autobaud lock.
    // SCI expects an 'a' or 'A' to lock the baud rate.
    //
    SCI_lockAutobaud(SCIA_BASE);
#endif
}

void checkSCIError(uint16_t rxStatus) {
    if((rxStatus & SCI_RXSTATUS_ERROR) != 0)
    {
        //
        //If Execution stops here there is some error
        //Analyze SCI_getRxStatus() API return value
        //
        ESTOP0;
    }
}

void SCI_readStruct(void *buffer, uint16_t buffSize) {
    uint16_t *memPtr = (uint16_t *) buffer;
    uint16_t i;
    for (i = 0; i < buffSize; i++) {
        uint16_t lower_byte, upper_byte;

        while(SCI_getRxFIFOStatus(SCIA_BASE) == SCI_FIFO_RX0);
        lower_byte = HWREGH(SCIA_BASE + SCI_O_RXBUF) & 0xFF;

        while(SCI_getRxFIFOStatus(SCIA_BASE) == SCI_FIFO_RX0);
        upper_byte = HWREGH(SCIA_BASE + SCI_O_RXBUF) & 0xFF;

        memPtr[i] = (upper_byte << 8) | lower_byte;
    }
}
void SCI_writeStruct(const void *buffer, uint16_t buffSize) {
    const uint16_t *memPtr = (const uint16_t *)buffer;
    uint16_t i;
    for (i = 0; i < buffSize; i++) {
        uint16_t word = memPtr[i];
        uint16_t lower_byte = word & 0xFF;
        uint16_t upper_byte = (word >> 8) & 0xFF;

        while(SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
        HWREGH(SCIA_BASE + SCI_O_TXBUF) = lower_byte;

        while(SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
        HWREGH(SCIA_BASE + SCI_O_TXBUF) = upper_byte;
    }
}

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
    uint16_t rxStatus = 0U;
    struct SimData data;
    float y;
    
    startup();

    for(;;)
    {


        rxStatus = SCI_getRxStatus(SCIA_BASE);
        checkSCIError(rxStatus);
        int i = 0;

        for (i = 0; i<16; i++) {
            while(SCI_getRxFIFOStatus(SCIA_BASE) == SCI_FIFO_RX0);
            __byte((uint8_t*) &data, i) = HWREGH(SCI_O_RXBUF + SCIA_BASE) & 0xff;
        }

        // SCI_readStruct(&x, sizeof(x));
        
        PI_c(&y, &data.u, data.gains, 1e-6);


        for (i = 0; i<4; i++) {
            while(SCI_getTxFIFOStatus(SCIA_BASE) == SCI_FIFO_TX16);
            HWREGH(SCI_O_TXBUF + SCIA_BASE )= __byte((uint8_t*) &y, i);
        }
        // SCI_writeStruct(&y, sizeof(y));


        loopCounter++;
    }
}

