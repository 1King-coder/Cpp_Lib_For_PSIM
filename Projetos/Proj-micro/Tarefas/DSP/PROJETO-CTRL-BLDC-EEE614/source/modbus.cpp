#include "modbus.h"

namespace peripherals 
{
    Uint16 holdingRegisters[NUM_HOLDING_REGS] = {0,0,0,0,0,0,0,0,0,0};
    Uint16 rxBuffer[256];
    Uint16 rxCount = 0;
    Uint16 frameComplete = 0;

    ModBusProtocol::ModBusProtocol (void) 
    {

    }

    Uint16 ModBusProtocol::calculate_CRC (Uint16 *buffer, Uint16 length) 
    {
        Uint16 crc = 0xFFFF;
        for (int pos = 0; pos < length; pos++) {
            crc ^= (Uint16) buffer[pos];
            for (int i = 8; i != 0; i--) {
                if ((crc & 0x0001) != 0) {
                    crc >>= 1;
                    crc ^= 0xA001;
                } else {
                    crc >>= 1;
                }
            }
        }
        return crc;
    }

    void ModBusProtocol::init_T35_Timer(void)
    {
        InitCpuTimers();
        ConfigCpuTimer(&CpuTimer0, 200, T35_TIMEOUT_US); 
        CpuTimer0Regs.TCR.bit.TSS = 1; // Stop timer initially
    }

    

    void ModBusProtocol::processModbusFrame (void)
    {
        // 1. Verify Slave ID
        if (this->rxBuffer[0] != MODBUS_SLAVE_ID) return;

        // 2. Verify CRC Integrity
        Uint16 receivedCRC = (this->rxBuffer[this->rxCount - 1] << 8) | this->rxBuffer[this->rxCount - 2];
        Uint16 calculatedCRC = this->calculate_CRC(this->rxBuffer, this->rxCount - 2);
        if (receivedCRC != calculatedCRC) return;

        // 3. Process Function Codes
        Uint16 functionCode = this->rxBuffer[1];
        Uint16 txBuffer[256];
        Uint16 txLen = 0;

        switch (functionCode)
        {
            case 0x03: // Read Holding Registers
            {
                Uint16 startAddr = (this->rxBuffer[2] << 8) | this->rxBuffer[3];
                Uint16 quantity = (this->rxBuffer[4] << 8) | this->rxBuffer[5];

                if ((startAddr + quantity) > 10) return; // Illegal Data Address protection

                txBuffer[0] = MODBUS_SLAVE_ID;
                txBuffer[1] = 0x03;
                txBuffer[2] = quantity * 2; // Byte count
                txLen = 3;

                for (int i = 0; i < quantity; i++) {
                    txBuffer[txLen++] = (this->holdingRegisters[startAddr + i] >> 8) & 0xFF; // High Byte
                    txBuffer[txLen++] = this->holdingRegisters[startAddr + i] & 0xFF;        // Low Byte
                }
                break;
            }
            case 0x06: // Write Single Register
            {
                Uint16 regAddr = (this->rxBuffer[2] << 8) | this->rxBuffer[3];
                Uint16 regValue = (this->rxBuffer[4] << 8) | this->rxBuffer[5];

                if (regAddr > 9) return; // Illegal Data Address protection

                this->holdingRegisters[regAddr] = regValue; // Apply change to DSP memory

                // Response to 0x06 is simply an exact echo of the request
                for (int i = 0; i < 6; i++) {
                    txBuffer[i] = this->rxBuffer[i];
                }
                txLen = 6;
                break;
            }
            case 0x10: // Write multiple Registers
            {
                Uint16 startAddr = (this->rxBuffer[2] << 8) | this->rxBuffer[3];
                Uint16 quantity  = (this->rxBuffer[4] << 8) | this->rxBuffer[5];
                Uint16 byteCount = this->rxBuffer[6];

                // 2. Safety Checks
                // Ensure they aren't trying to write past the end of your array
                if ((startAddr + quantity) > NUM_HOLDING_REGS) return; 
                
                // Ensure the byte count matches the quantity of registers (2 bytes per reg)
                if (byteCount != (quantity * 2)) return;

                // 3. Write the data into the holding registers
                // The actual data starts at rxBuffer[7]
                int dataIndex = 7;
                for (int i = 0; i < quantity; i++) 
                {
                    // Reconstruct the 16-bit value from the two 8-bit bytes
                    Uint16 regValue = (this->rxBuffer[dataIndex] << 8) | this->rxBuffer[dataIndex + 1];
                    
                    // Store it in the DSP's memory
                    this->holdingRegisters[startAddr + i] = regValue;
                    
                    // Move to the next pair of bytes
                    dataIndex += 2;
                }

                // 4. Format the Response (Echo the first 6 bytes)
                for (int i = 0; i < 6; i++) {
                    txBuffer[i] = this->rxBuffer[i];
                }
                txLen = 6;
                break;
            }
            default:
                return; // Unsupported Function Code
        }

        // Append CRC to the outgoing response and transmit
        Uint16 txCRC = this->calculate_CRC(txBuffer, txLen);
        txBuffer[txLen++] = txCRC & 0xFF;        // Low Byte
        txBuffer[txLen++] = (txCRC >> 8) & 0xFF; // High Byte

        this->sendModbusResponse(txBuffer, txLen);
    }

    void ModBusProtocol::sendModbusResponse(Uint16 *data, Uint16 length)
    {
        for (int i = 0; i < length; i++)
        {
            while (SciaRegs.SCIFFTX.bit.TXFFST >= 16) {}
            SciaRegs.SCITXBUF.all = data[i] & 0xFF;
        }
    }


}
