#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "global_definitions.h"
#include <F2837xD_epwm.h>

namespace peripherals {
    class PwmProj
    {
        public:
            PwmProj(void);
            volatile struct EPWM_REGS *ePwmRegisters[NUMBER_OF_PWM_MODULES];
            Uint16 pwmTimeBasePeriod[NUMBER_OF_PWM_MODULES];
            Uint16 switching_frequency_Hz[NUMBER_OF_PWM_MODULES];
            float time_base_clock_Hz[NUMBER_OF_PWM_MODULES];
            bool double_sampling_frequency;
            PINT isr_pointer[NUMBER_OF_PWM_MODULES];
            union
            {
                Uint16 all;
                struct
                {
                    bool interruptPwmModule1 :1;
                    bool interruptPwmModule2 :1;
                    bool interruptPwmModule3 :1;
                    bool interruptPwmModule4 :1;
                    bool interruptPwmModule5 :1;
                    bool interruptPwmModule6 :1;
                    bool interruptPwmModule7 :1;
                    bool interruptPwmModule8 :1;
                    bool interruptPwmModule9 :1;
                    bool interruptPwmModule10 :1;
                    bool interruptPwmModule11 :1;
                    bool interruptPwmModule12 :1;
                } bit;
            } interruptsEnableBits;

            enum PresetConfigurations
		    {
                triangle_interrupt_on_zero = 0,
                triangle_interrupt_on_zero_and_top
            };
            void set_pwm_configuration(PresetConfigurations pwmPresetConfiguration);
            void init();
            void config_pwm_divs (const unsigned int module, const unsigned int HSPCLKDIV, const unsigned int CLKDIV);
            void set_switching_frequency_Hz(const unsigned int module, float switching_frequency);
            void set_pwm_value(const unsigned int module, float value);
            void set_gpio(const unsigned int module);
            void set_interrupt(volatile PINT isrPtr, const unsigned int module);
            void enable_pwm_output(const unsigned int module);
            void disable_pwm_output(const unsigned int module);
            void enable_interrupt(void);
            void init_pwm_triangle_interrupt_on_zero(const unsigned int module);
            void setSynchronizationWithModule1 (Uint16 moduleIndex, bool enable);
    
        private:
    };
}
