#include "F28x_Project.h"
#include "pwm.h"
#include "ecap1.h"

namespace project {
    class BLDCDriveAndControlProject
    {
        public:
            struct BLDC_input_data 
            {
                float w_m_ref;
                float w_m_feedback;
                float i_in_feedback;  
            };

            struct DSP_Response {
                float ref;
                float w_m_calc;
            };
            float w_m_x1, w_m_y1, i_in_x1, i_in_y1, i_in_ref;
            BLDC_input_data circuit_data;
            DSP_Response response;
            peripherals::PwmProj pwm;
            peripherals::EcapProj ecap;
            Uint32 captured_ticks;
            float sim_hall_sensor_freq_read;
            float w_m_rpm;


            BLDCDriveAndControlProject(void);
            void project_startup (void);
            void set_interrupt (volatile PINT *pieVectorDest, PINT newIsr, 
                                Uint16 coreIerMask, volatile Uint16 *pieIerReg, Uint16 pieIerMask);
            void loop (void);
            void w_m_pi_control (BLDC_input_data *data, float *ref);
            float calc_simulated_hall_sensor_freq (float w_m);
            float calc_simulated_w_m_feedback (float freq);
            void setup_pwm (void);
            void setup_ecap (void);
        private:
    };
}
