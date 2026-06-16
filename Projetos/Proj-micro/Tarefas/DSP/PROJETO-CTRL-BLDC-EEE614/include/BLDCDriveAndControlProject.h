#include "F28x_Project.h"
namespace project {
class BLDCDriveAndControlProject
    {
        public:
            struct BLDC_input_data {
              float w_m_ref;
              float w_m_feedback;
              float i_in_feedback;  
            };
            float w_m_x1, w_m_y1, i_in_x1, i_in_y1;
            BLDCDriveAndControlProject(void);
            void project_startup (void);
            void set_interrupt (volatile PINT *pieVectorDest, PINT newIsr, 
                                                    Uint16 coreIerMask, volatile Uint16 *pieIerReg, Uint16 pieIerMask);
            void loop (void);
            void BLDCDriveAndControlProject::w_m_pi_control (project::BLDCDriveAndControlProject::BLDC_input_data *data, float *ref);
            
        private:
    };
}
