#include "eCap1.h"

namespace peripherals {

    EcapProj::EcapProj(void) {
        // Map register pointers to the correct hardware memory addresses
        this->eCapRegisters[0] = &ECap1Regs;
        this->eCapRegisters[1] = &ECap2Regs;
        this->eCapRegisters[2] = &ECap3Regs;
        this->eCapRegisters[3] = &ECap4Regs;
        this->eCapRegisters[4] = &ECap5Regs;
        this->eCapRegisters[5] = &ECap6Regs;
        this->interruptsEnableBits.all = 0;
    }

    void EcapProj::set_gpio(const unsigned int module, const Uint16 gpio_pin) 
    {
        EALLOW;
        // 1. Configure pin as a standard asynchronous GPIO input
        // GPIO_SetupPinMux(gpio_pin, GPIO_MUX_CPU1, 0); 
        // GPIO_SetupPinOptions(gpio_pin, GPIO_INPUT, GPIO_ASYNC);

        // 2. On the F28379D, eCAP inputs are strictly routed through the Input X-BAR.
        // Documentation mapping: eCAP1 = INPUT7, eCAP2 = INPUT8, ..., eCAP4 = INPUT10
        switch (module)
        {
            case 1: 
                InputXbarRegs.INPUT7SELECT = gpio_pin; 
                break;
            case 2: 
                InputXbarRegs.INPUT8SELECT = gpio_pin; 
                break;
            case 3: 
                InputXbarRegs.INPUT9SELECT = gpio_pin; 
                break;
            case 4: 
                InputXbarRegs.INPUT10SELECT = gpio_pin; 
                break;
            default: 
                break;
        }
        EDIS;
    }

    void EcapProj::init_delta_mode(const unsigned int module) 
    {
        unsigned int moduleIndex = module - 1;

        EALLOW;
        // Stop the counter before configuration
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.TSCTRSTOP = 0;

        // Configure Capture Mode (Not APWM mode)
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.CAP_APWM = 0;     // Capture mode
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.CONT_ONESHT = 0;  // Continuous mode
        
        // Event 1 Configuration: Trigger on Rising edge, reset counter (Delta mode)
        this->eCapRegisters[moduleIndex]->ECCTL1.bit.PRESCALE = 0;     // Divide by 1 (no prescale)
        this->eCapRegisters[moduleIndex]->ECCTL1.bit.CAP1POL = 0;      // Rising edge
        this->eCapRegisters[moduleIndex]->ECCTL1.bit.CTRRST1 = 1;      // Reset counter after Event 1
        this->eCapRegisters[moduleIndex]->ECCTL1.bit.CAPLDEN = 1;      // Enable CAP1-4 register loads


        // Set wrap-around at Event 1 (we only care about the period between 2 edges)
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.STOP_WRAP = 0;    // Wrap after Capture Event 1
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.SYNCI_EN = 0;     // Disable sync-in

        // Enable Interrupt on Capture Event 1
        this->eCapRegisters[moduleIndex]->ECEINT.bit.CEVT1 = 1;

        // Start the counter
        this->eCapRegisters[moduleIndex]->ECCTL2.bit.TSCTRSTOP = 1;
        EDIS;
    }

    void EcapProj::set_interrupt(volatile PINT isrPtr, const unsigned int module)
    {
        EALLOW;
        switch (module)
        {
            case 1:
                PieVectTable.ECAP1_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule1 = true;
                break;
            case 2:
                PieVectTable.ECAP2_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule2 = true;
                break;
            case 3:
                PieVectTable.ECAP3_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule3 = true;
                break;
            case 4:
                PieVectTable.ECAP4_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule4 = true;
                break;
            case 5:
                PieVectTable.ECAP5_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule5 = true;
                break;
            case 6:
                PieVectTable.ECAP6_INT = isrPtr;
                this->interruptsEnableBits.bit.interruptEcapModule6 = true;
                break;
            default:
                break;
        }
        EDIS;
    }

    void EcapProj::enable_interrupt(void)
    {
        // eCAP interrupts are mapped to PIE Group 4 on the F28379D
        if (this->interruptsEnableBits.bit.interruptEcapModule1) PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
        if (this->interruptsEnableBits.bit.interruptEcapModule2) PieCtrlRegs.PIEIER4.bit.INTx2 = 1;
        if (this->interruptsEnableBits.bit.interruptEcapModule3) PieCtrlRegs.PIEIER4.bit.INTx3 = 1;
        if (this->interruptsEnableBits.bit.interruptEcapModule4) PieCtrlRegs.PIEIER4.bit.INTx4 = 1;
        if (this->interruptsEnableBits.bit.interruptEcapModule5) PieCtrlRegs.PIEIER4.bit.INTx5 = 1;
        if (this->interruptsEnableBits.bit.interruptEcapModule6) PieCtrlRegs.PIEIER4.bit.INTx6 = 1;

        // Enable Group 4 Interrupts globally
        IER |= M_INT4;
    }

    Uint32 EcapProj::get_period(const unsigned int module)
    {
        unsigned int moduleIndex = module - 1;
        // In delta mode with wrap on CEVT1, the captured period resides strictly in CAP1
        return this->eCapRegisters[moduleIndex]->CAP1;
    }
    
    void EcapProj::clear_interrupt_flag(const unsigned int module)
    {
        unsigned int moduleIndex = module - 1;
        // Clear the Global eCAP Interrupt Flag and CEVT1 Flag
        this->eCapRegisters[moduleIndex]->ECCLR.bit.CEVT1 = 1;
        this->eCapRegisters[moduleIndex]->ECCLR.bit.INT = 1;
    }

}
