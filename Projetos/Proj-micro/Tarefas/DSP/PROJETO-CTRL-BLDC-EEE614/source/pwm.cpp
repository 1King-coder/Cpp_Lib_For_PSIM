#include "F28x_Project.h"
#include "pwm.h"
#include <F2837xD_epwm.h>
#include "global_definitions.h"


namespace peripherals {

    PwmProj::PwmProj (void) {
        this->ePwmRegisters[0] = &EPwm1Regs;
        this->ePwmRegisters[1] = &EPwm2Regs;
        this->ePwmRegisters[2] = &EPwm3Regs;
        this->ePwmRegisters[3] = &EPwm4Regs;
        this->ePwmRegisters[4] = &EPwm5Regs;
        this->ePwmRegisters[5] = &EPwm6Regs;
        this->ePwmRegisters[6] = &EPwm7Regs;
        this->ePwmRegisters[7] = &EPwm8Regs;
        this->ePwmRegisters[8] = &EPwm9Regs;
        this->ePwmRegisters[9] = &EPwm10Regs;
        this->ePwmRegisters[10] = &EPwm11Regs;
        this->ePwmRegisters[11] = &EPwm12Regs;
        

        this->pwmTimeBasePeriod = 0;

        for (unsigned int moduleIndex = 0; moduleIndex < NUMBER_OF_PWM_MODULES; moduleIndex++)
        {

            this->time_base_clock_Hz = DSP_CLOCK / (14 * 128);
                    // / powi(2.0f, this->ePwmRegisters[moduleIndex]->TBCTL.bit.HSPCLKDIV)
                    // / powi(2.0f, this->ePwmRegisters[moduleIndex]->TBCTL.bit.CLKDIV);
        }
    }

    void PwmProj::set_pwm_configuration(PwmProj::PresetConfigurations pwmPresetConfiguration) {
        switch (pwmPresetConfiguration)
        {
            case triangle_interrupt_on_zero:
                this->double_sampling_frequency = false;
                break;
            case triangle_interrupt_on_zero_and_top:
                this->double_sampling_frequency = true;
                break;
            default:
                break;
        }
    };


    void PwmProj::set_interrupt(volatile PINT isrPtr, const unsigned int module)
    {

        EALLOW;

        switch (module)
        {
        case 1:
            PieVectTable.EPWM1_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule1 = true;
            break;
        case 2:
            PieVectTable.EPWM2_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule2 = true;
            break;
        case 3:
            PieVectTable.EPWM3_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule3 = true;
            break;
        case 4:
            PieVectTable.EPWM4_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule4 = true;
            break;
        case 5:
            PieVectTable.EPWM5_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule5 = true;
            break;
        case 6:
            PieVectTable.EPWM6_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule6 = true;
            break;
        case 7:
            PieVectTable.EPWM7_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule7 = true;
            break;
        case 8:
            PieVectTable.EPWM8_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule8 = true;
            break;
        case 9:
            PieVectTable.EPWM9_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule9 = true;
            break;
        case 10:
            PieVectTable.EPWM10_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule10 = true;
            break;
        case 11:
            PieVectTable.EPWM11_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule11 = true;
            break;
        case 12:
            PieVectTable.EPWM12_INT = isrPtr;
            this->interruptsEnableBits.bit.interruptPwmModule12 = true;
            break;

        default:
            break;
        }
        EDIS;

    }

    void PwmProj::enable_interrupt(void)
    {
        if (this->interruptsEnableBits.bit.interruptPwmModule1)
        {
            PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
        }
        if (this->interruptsEnableBits.bit.interruptPwmModule2)
        {
            PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule3)
        {
            PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule4)
        {
            PieCtrlRegs.PIEIER3.bit.INTx4 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule5)
        {
            PieCtrlRegs.PIEIER3.bit.INTx5 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule6)
        {
            PieCtrlRegs.PIEIER3.bit.INTx6 = 1;
        }
        if (this->interruptsEnableBits.bit.interruptPwmModule7)
        {
            PieCtrlRegs.PIEIER3.bit.INTx7 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule8)
        {
            PieCtrlRegs.PIEIER3.bit.INTx8 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule9)
        {
            PieCtrlRegs.PIEIER3.bit.INTx9 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule10)
        {
            PieCtrlRegs.PIEIER3.bit.INTx10 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule11)
        {
            PieCtrlRegs.PIEIER3.bit.INTx11 = 1;
        }

        if (this->interruptsEnableBits.bit.interruptPwmModule12)
        {
            PieCtrlRegs.PIEIER3.bit.INTx12 = 1;
        }

        IER |= M_INT3;
    }

    void PwmProj::init() {
        EALLOW;
        CpuSysRegs.PCLKCR0.bit.HRPWM = 1;
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
        EDIS;

        for (unsigned int module = 1; module <= NUMBER_OF_PWM_MODULES; module++)
        {
            this->init_pwm_triangle_interrupt_on_zero(module);
        }

        EALLOW;
        CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
        EDIS;
    };
    void PwmProj::set_switching_frequency_Hz(float switching_frequency) 
    {
        this->pwmTimeBasePeriod = (Uint16) (this->time_base_clock_Hz / (switching_frequency * 4.0f));
        
        this->switching_frequency_Hz = this->time_base_clock_Hz / (4.0f * (float) this->pwmTimeBasePeriod);
    };
    void PwmProj::set_pwm_value(const unsigned int module, float value) 
    {
        this->ePwmRegisters[module - 1]->CMPA.bit.CMPA = (Uint16) ((float)this->pwmTimeBasePeriod * (1.0f - value) * 0.5f);
    };

    void PwmProj::set_gpio (const unsigned int module) 
    {
        EALLOW;
        switch (module)
        {
        case 1:
            //Set mux to EPWM1A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;

            //Set mux to EPWM1B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
            break;
        case 2:
            //Set mux to EPWM2A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;

            //Set mux to EPWM2B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
            break;
        case 3:
            //Set mux to EPWM3A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO4 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;

            //Set mux to EPWM3B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO5 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;
            break;
        case 4:
            //Set mux to EPWM4A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;

            //Set mux to EPWM4B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO7 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;
            break;
        case 5:
            //Set mux to EPWM5A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO8 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;

            //Set mux to EPWM5B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO9 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;
            break;
        case 6:
            //Set mux to EPWM6A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;

            //Set mux to EPWM6B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;
            break;
        case 7:
            //Set mux to EPWM7A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;

            //Set mux to EPWM7B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;
            break;
        case 8:
            //Set mux to EPWM8A
            GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;

            //Set mux to EPWM8B
            GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = 0;
            GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;
            break;
        case 9:
            //Set mux to EPWM9A
            GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1;

            //Set mux to EPWM9B
            GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;
            break;
        case 10:
            //Set mux to EPWM10A
            GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;

            //Set mux to EPWM10B
            GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;
            break;
        case 11:
            //Set mux to EPWM11A
            GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;

            //Set mux to EPWM11B
            GpioCtrlRegs.GPAGMUX2.bit.GPIO21 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;
            break;
        case 12:
            //Set mux to EPWM12A
            GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;

            //Set mux to EPWM12B
            GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 0;
            GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;
            GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;
            break;
        default:
            break;
        }
        EDIS;
    };

    void PwmProj::enable_pwm_output(const unsigned int module)
    {
        unsigned int moduleIndex = module - 1;

        EALLOW;
        this->ePwmRegisters[moduleIndex]->TZCLR.bit.OST = 1;
        EDIS;
    }

    void PwmProj::disable_pwm_output (const unsigned int module) {
        unsigned int moduleIndex = module - 1;
        EALLOW;
        this->ePwmRegisters[moduleIndex]->TZFRC.bit.OST = 1;
        EDIS;
    }

    void PwmProj::init_pwm_triangle_interrupt_on_zero(const unsigned int module)
    {

        unsigned int moduleIndex = module - 1;

        /// High Speed Time-base Clock Prescale Bits
        this->ePwmRegisters[moduleIndex]->TBCTL.bit.HSPCLKDIV = 7;

        /// High Speed Time-base Clock Prescale Bits; PS=1
        this->ePwmRegisters[moduleIndex]->TBCTL.bit.CLKDIV = 7;
        /// Set timer period
        this->ePwmRegisters[moduleIndex]->TBPRD = this->pwmTimeBasePeriod;

        this->ePwmRegisters[moduleIndex]->TBCTL.bit.PRDLD = TB_SHADOW;

        /// Count UP DOWN
        this->ePwmRegisters[moduleIndex]->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

        /**
         * value set for time base counter TBCTR when
         * synchronizing event happens. may be used to set
         * phase difference between carrier signals.
         */
        this->ePwmRegisters[moduleIndex]->TBPHS.bit.TBPHS = 0;
        /// clear TB counter
        this->ePwmRegisters[moduleIndex]->TBCTR = 0;

        //  Enable INT
        switch (module)
        {
        case 1:
            this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTSEL = ET_CTR_ZERO;
            this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTEN = 1;
            break;

        case 2:
            if (this->double_sampling_frequency)
            {
                this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTSEL = ET_CTR_PRD;
                this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTEN = 1;
            }
            else
            {
                this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTEN = 0;
            }

            break;

        default:
            this->ePwmRegisters[moduleIndex]->ETSEL.bit.INTEN = 0;
            break;
        }

        /// Generate INT on 1st event
        this->ePwmRegisters[moduleIndex]->ETPS.bit.INTPRD = ET_1ST;

        /// disable SOCA
        this->ePwmRegisters[moduleIndex]->ETSEL.bit.SOCAEN = ET_DISABLE;

        /// disable SOCB
        this->ePwmRegisters[moduleIndex]->ETSEL.bit.SOCBEN = ET_DISABLE;

        /**
         * Atualização do Registrador de Comparação
         * Carrega do registrador de comparação A no zero e
         * no topo da triangular
         */
        this->ePwmRegisters[moduleIndex]->CMPCTL.bit.SHDWAMODE = CC_SHADOW;

        if (this->double_sampling_frequency)
        {
            this->ePwmRegisters[moduleIndex]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;

            this->ePwmRegisters[moduleIndex]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
        }
        else
        {
            this->ePwmRegisters[moduleIndex]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

            this->ePwmRegisters[moduleIndex]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
        }

        this->ePwmRegisters[moduleIndex]->CMPCTL.bit.SHDWBMODE = CC_SHADOW;

        //  Set actions for A register
        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.CAU = AQ_CLEAR;

        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.CAD = AQ_SET;

        /*
         * B channel action qualifier will not be used. B channel outputs will be
         * generated by dead-band submodule from the A channel action qualifier
         * (Figure 30, Figure 31, SPRUG04A)
         */
        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.CAU = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.CAD = AQ_NO_ACTION;

        /// B compare register will not be used for pwm output
        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.CBD = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.CBU = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.CBD = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.CBU = AQ_NO_ACTION;

        /// no specific actions for zero or period (top)
        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.PRD = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLA.bit.ZRO = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.PRD = AQ_NO_ACTION;

        this->ePwmRegisters[moduleIndex]->AQCTLB.bit.ZRO = AQ_NO_ACTION;


        /// disable pwm chopping
        this->ePwmRegisters[moduleIndex]->PCCTL.bit.CHPEN = CHP_DISABLE;

        /// Trip Zone submodule forces low state on trip
        /**
         * Trip zone configured as a protection mechanism to
         * set pwm output to low state.
         */
        EALLOW;
        this->ePwmRegisters[moduleIndex]->TZCTL.bit.TZA = TZ_FORCE_LO;

        this->ePwmRegisters[moduleIndex]->TZCTL.bit.TZB = TZ_FORCE_LO;
        EDIS;

        /// CONFIGURACAO PARA COLOCAR NIVEL ZERO NO PWM
        this->disable_pwm_output(module);

        /// synchronization option
        this->setSynchronizationWithModule1(moduleIndex, false);
    }

    void PwmProj::setSynchronizationWithModule1(Uint16 moduleIndex, bool enable)
    {
        if (enable)
        {
            if (moduleIndex == 0)
            {

                this->ePwmRegisters[moduleIndex]->TBCTL.bit.PHSEN = TB_DISABLE;


                this->ePwmRegisters[moduleIndex]->TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
            }
            else
            {
                /// Phase loading
                this->ePwmRegisters[moduleIndex]->TBCTL.bit.PHSEN = TB_ENABLE;

                this->ePwmRegisters[moduleIndex]->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

                /*
                 * direction counter is going after
                 * synchronization*/

                this->ePwmRegisters[moduleIndex]->TBCTL.bit.PHSDIR = TB_DOWN;
            }
        }
        else
        {
            /// Phase loading
            this->ePwmRegisters[moduleIndex]->TBCTL.bit.PHSEN = TB_DISABLE;

            ///  Synchronization ouput mux (disable)
            this->ePwmRegisters[moduleIndex]->TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
        }
    }

}
