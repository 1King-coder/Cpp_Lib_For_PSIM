#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "global_definitions.h"
#include <F2837xD_ecap.h>

namespace peripherals {
    class EcapProj
    {
        public:
            EcapProj(void);

            volatile struct ECAP_REGS *eCapRegisters[NUMBER_OF_ECAP_MODULES];
            PINT isr_pointer[NUMBER_OF_ECAP_MODULES];
            
            union
            {
                Uint16 all;
                struct
                {
                    bool interruptEcapModule1 :1;
                    bool interruptEcapModule2 :1;
                    bool interruptEcapModule3 :1;
                    bool interruptEcapModule4 :1;
                    bool interruptEcapModule5 :1;
                    bool interruptEcapModule6 :1;
                } bit;
            } interruptsEnableBits;

            // Base Configurations
            void init_delta_mode(const unsigned int module);
            
            // Core Peripheral Functions
            void set_gpio(const unsigned int module, const Uint16 gpio_pin);
            void set_interrupt(volatile PINT isrPtr, const unsigned int module);
            void enable_interrupt(void);
            
            // Operational Functions
            Uint32 get_period(const unsigned int module);
            void clear_interrupt_flag(const unsigned int module);

        private:
    };
}
