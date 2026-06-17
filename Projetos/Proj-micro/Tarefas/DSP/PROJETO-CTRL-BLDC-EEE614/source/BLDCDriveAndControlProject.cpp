#include "F28x_Project.h"
#include "sci_com.h"
#include "control_logics.h"
#include "BLDC_basis.h"
#include "BLDCDriveAndControlProject.h"
#include "global_definitions.h"





namespace project {
    BLDCDriveAndControlProject::BLDC_input_data circuit_data;
    float i_in_ref = 0;
    float w_m_x1 = 0;
    float w_m_y1 = 0;
    float i_in_x1 = 0;
    float i_in_y1 = 0;
    peripherals::PwmProj pwm;
    peripherals::EcapProj ecap;
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
        for (;;) {
            
        }
    }

    void BLDCDriveAndControlProject::w_m_pi_control (project::BLDCDriveAndControlProject::BLDC_input_data *data, float *ref) 
    {
        float w_m_err = data->w_m_ref - this->w_m_rpm;
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

        PI(w_pi_cts, &i_in_ref, w_m_err, &w_m_y1, &w_m_x1, SAMPLE_FREQ);

        float i_in_err = i_in_ref - data->i_in_feedback;

        PI(i_in_pi_cts, ref, i_in_err, &i_in_y1, &i_in_x1, SAMPLE_FREQ);
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
        this->pwm.set_switching_frequency_Hz(0);
        this->pwm.init();
        this->pwm.set_gpio(1);
        this->pwm.set_pwm_value(1, 0);
        this->pwm.enable_pwm_output(1);
    }

    void BLDCDriveAndControlProject::setup_ecap (void) 
    {
        this->ecap.set_gpio(1, 24);
        this->ecap.init_delta_mode(1);
    }
    
    
}




