#define SCI_TARGET_BAUD ((float) 115200.0f)
#define DSP_CLOCK ((float) 200e6f)
#define KP_W ((float) 3.1107f)
#define KI_W ((float) 15.363951f)
#define W_PI_HLIMIT ((float) 10.0f)
#define W_PI_LLIMIT ((float) 0)
#define KP_I ((float) 0.24f)
#define KI_I ((float) 280.0f)
#define I_PI_HLIMIT ((float) 36.0f)
#define I_PI_LLIMIT ((float) 0)
#define SAMPLE_FREQ ((float) 1/(1e3))
#define IS_USING_MODBUS ((bool) 1)
#define RECEIVED_DATA_SIZE ((int) (IS_USING_MODBUS ? 1 : 12))
#define NUMBER_OF_PWM_MODULES ((unsigned int) 12)
#define NUMBER_OF_MACHINE_POLE_PAIRS ((unsigned int) 10)
#define NUMBER_OF_ECAP_MODULES ((unsigned int) 6)
#define NUM_HOLDING_REGS 10

#include "F28x_Project.h"
void set_interrupt (volatile PINT *pieVectorDest, PINT newIsr, 
                    Uint16 coreIerMask, volatile Uint16 *pieIerReg, Uint16 pieIerMask);
                    

float powi(float base, unsigned short exponent);
