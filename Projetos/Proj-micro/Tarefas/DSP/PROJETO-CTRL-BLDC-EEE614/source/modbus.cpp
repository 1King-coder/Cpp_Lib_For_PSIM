#include "modbus.h"


uint16_t calculate_CRC (uint8_t *buffer, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < length; pos++) {
        crc ^= (uint16_t) buffer[pos];
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

uint16_t HoldingRegisters[NUM_HOLDING_REGS];

void Process_Modbus(void) {
    if (!FrameReceived) return;

    // 1. Verify Address
    if (RxBuffer[0] != SLAVE_ADDRESS) {
        RxIndex = 0;
        FrameReceived = false;
        return;
    }

    // 2. Verify CRC
    uint16_t received_crc = (RxBuffer[RxIndex - 1] << 8) | RxBuffer[RxIndex - 2];
    uint16_t calculated_crc = Calculate_CRC(RxBuffer, RxIndex - 2);

    if (received_crc == calculated_crc) {
        uint8_t function_code = RxBuffer[1];
        uint8_t tx_buffer[32];
        uint8_t tx_len = 0;

        if (function_code == 0x03) { // Read Holding Registers
            uint16_t start_addr = (RxBuffer[2] << 8) | RxBuffer[3];
            uint16_t num_regs = (RxBuffer[4] << 8) | RxBuffer[5];

            tx_buffer[tx_len++] = SLAVE_ADDRESS;
            tx_buffer[tx_len++] = function_code;
            tx_buffer[tx_len++] = num_regs * 2; // Byte count

            for (int i = 0; i < num_regs; i++) {
                uint16_t reg_val = HoldingRegisters[start_addr + i];
                tx_buffer[tx_len++] = (reg_val >> 8) & 0xFF; // High byte
                tx_buffer[tx_len++] = reg_val & 0xFF;        // Low byte
            }

            // Append CRC
            uint16_t crc = Calculate_CRC(tx_buffer, tx_len);
            tx_buffer[tx_len++] = crc & 0xFF;
            tx_buffer[tx_len++] = (crc >> 8) & 0xFF;

            // Send via SCI
            for (int i = 0; i < tx_len; i++) {
                while (SciaRegs.SCICTL2.bit.TXRDY == 0);
                SciaRegs.SCITXBUF.all = tx_buffer[i];
            }
        }
    }

    // Reset for next frame
    RxIndex = 0;
    FrameReceived = false;
}
