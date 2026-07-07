#include "F28x_Project.h"
#include "sci_com.h"
#include "control_logics.h"
#include "BLDC_basis.h"
#include "BLDCDriveAndControlProject.h"
#include "global_definitions.h"



namespace project {
    BLDCDriveAndControlProject::BLDC_input_data measures;
    BLDCDriveAndControlProject::DSP_Response response;
    float i_in_ref = 0;
    float w_m_x1 = 0;
    float w_m_y1 = 0;
    float i_in_x1 = 0;
    float i_in_y1 = 0;
    peripherals::PwmProj pwm;
    peripherals::EcapProj ecap;
    peripherals::ModBusProtocol mb;
    Uint32 captured_ticks = 0;
    float sim_hall_sensor_freq_read = 0;
    float w_m_rpm = 0;

    BLDCDriveAndControlProject::BLDCDriveAndControlProject (void) 
    {
        this->project_startup();
    }

    void BLDCDriveAndControlProject::project_startup (void) 
    {
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
        scia_init();

    }

    void BLDCDriveAndControlProject::loop () 
    {
        
        for (;;) 
        {
            if (this->mb.frameComplete == 1)
            {
                this->mb.processModbusFrame();
                
                this->mb.rxCount = 0;
                this->mb.frameComplete = 0;

                this->measures.i_in_feedback = (float) this->mb.holdingRegisters[0] / 100;
                this->measures.w_m_feedback = (float) this->mb.holdingRegisters[1] / 100;
                this->measures.w_m_ref = (float) this->mb.holdingRegisters[2] / 100;
                this->w_m_pi_control();
                this->mb.holdingRegisters[3] = (Uint16) (this->results.ref * 100);
                this->mb.holdingRegisters[4] = (Uint16) (this->results.w_m_calc * 100);
            }
        }
    }

    void BLDCDriveAndControlProject::w_m_pi_control () 
    {
        float w_m_err = this->measures.w_m_ref - this->w_m_rpm;
        float i_in_ref;
        PI_CTS w_pi_cts, i_in_pi_cts;

        w_pi_cts.hLim = W_PI_HLIMIT;
        w_pi_cts.lLim = W_PI_LLIMIT;
        w_pi_cts.kp = KP_W;
        w_pi_cts.ki = KI_W;

        i_in_pi_cts.hLim = I_PI_HLIMIT;
        i_in_pi_cts.lLim = I_PI_LLIMIT;
        i_in_pi_cts.kp = KP_I;
        i_in_pi_cts.ki = KI_I;

        PI(w_pi_cts, &i_in_ref, w_m_err, &w_m_y1, &w_m_x1, SAMPLE_PERIOD);

        float i_in_err = i_in_ref - this->measures.i_in_feedback;

        PI(i_in_pi_cts, &this->results.ref, i_in_err, &i_in_y1, &i_in_x1, SAMPLE_PERIOD);
    }

    float BLDCDriveAndControlProject::calc_simulated_hall_sensor_freq (float w_m) 
    {
        return w_m / 60 * NUMBER_OF_MACHINE_POLE_PAIRS;
    }

    float BLDCDriveAndControlProject::calc_simulated_w_m_feedback (float freq) 
    {
        return freq * 60 / NUMBER_OF_MACHINE_POLE_PAIRS;
    }

    void BLDCDriveAndControlProject::setup_pwm (void) 
    {
        this->pwm.set_pwm_configuration(peripherals::PwmProj::triangle_interrupt_on_zero);
        this->pwm.config_pwm_divs(1, 7, 7);
        this->pwm.set_switching_frequency_Hz(1, 0);
        this->pwm.config_pwm_divs(2, 1, 0);
        this->pwm.set_switching_frequency_Hz(2, 20e3);
        this->pwm.init();
        this->pwm.set_gpio(1);
        this->pwm.set_gpio(2);
        this->pwm.set_pwm_value(1, 0);
        this->pwm.set_pwm_value(2, 0);
    }

    void BLDCDriveAndControlProject::setup_ecap (void) 
    {
        this->ecap.set_gpio(1, 15);
        this->ecap.init_delta_mode(1);
    }

    void BLDCDriveAndControlProject::setup_mb (void) 
    {
        this->mb.init_T35_Timer();
    }
    
    
}




