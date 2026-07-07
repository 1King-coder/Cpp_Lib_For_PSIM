#include "global_definitions.h"

namespace peripherals 
{
    class ModBusProtocol 
    {
        public:
            ModBusProtocol(void);
            Uint16 holdingRegisters[NUM_HOLDING_REGS];
            Uint16 rxBuffer[256];
            Uint16 rxCount;
            Uint16 frameComplete;
            Uint16 calculate_CRC (Uint16 *buffer, Uint16 length);
            void init_T35_Timer(void);
            void processModbusFrame(void);
            void sendModbusResponse(Uint16 *data, Uint16 length);
    };
}


