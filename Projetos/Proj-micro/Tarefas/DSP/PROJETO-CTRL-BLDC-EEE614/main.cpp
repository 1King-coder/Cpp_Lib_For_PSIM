
#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "BLDCDriveAndControlProject.h"
#include "sci_com.h"
#include "global_definitions.h"


project::BLDCDriveAndControlProject proj;

__interrupt void receiveModbusData(void)
{
    // Read the byte that just arrived
    if(proj.mb.rxCount < 256) {
        proj.mb.rxBuffer[proj.mb.rxCount++] = SciaRegs.SCIRXBUF.all & 0xFF;
    } else {
        Uint16 dummy = SciaRegs.SCIRXBUF.all; // Prevent buffer overflow
    }

    // Reset and Restart the T3.5 Timer
    CpuTimer0Regs.TCR.bit.TRB = 1; // Reload timer
    CpuTimer0Regs.TCR.bit.TSS = 0; // Start timer

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

__interrupt void timer_isr(void)
{
    // If the timer triggers, the line has been silent for 4ms. Frame is done.
    CpuTimer0Regs.TCR.bit.TSS = 1; // Stop timer

    if(proj.mb.rxCount > 3) // Minimum Modbus frame is 4 bytes (ID, FC, CRC1, CRC2)
    {
        proj.mb.frameComplete = 1; 
    } else {
        proj.mb.rxCount = 0; // Garbage data, reset
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// __interrupt void processDataReceiveINT (void) {
//     receiveData(&proj.circuit_data, 12);
//     proj.w_m_pi_control(&proj.circuit_data, &proj.response.ref);
//     sendData(&proj.response, 8);
//     if (rxIsrCounter == 0) {
//         proj.pwm.enable_pwm_output(1); 
//     }
    

//     SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
//     SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
//     PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
//     rxIsrCounter++;
// }

__interrupt void simulateHallSensorPulses (void) 
{
    float sim_hall_freq = proj.calc_simulated_hall_sensor_freq(proj.measures.w_m_feedback);
    proj.pwm.set_switching_frequency_Hz(1, sim_hall_freq);

    if (proj.pwm.switching_frequency_Hz[0] <= 0)
    {
        proj.pwm.disable_pwm_output(1);
        proj.results.w_m_calc = proj.w_m_rpm = 0;
    } else {
        proj.pwm.enable_pwm_output(1);
    }

    EPwm1Regs.TBPRD = proj.pwm.pwmTimeBasePeriod[0]; 

    proj.pwm.set_pwm_value(1, 0);

    
    EPwm2Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    
}

__interrupt void getWmFromHallSensorPulses (void)
{
    // 1. Get the period in raw SYSCLK ticks from the CAP1 register
    proj.captured_ticks = proj.ecap.get_period(1);

    // 2. Calculate the mechanical velocity (RPM) safely
    if (proj.captured_ticks > 0) 
    {
        // f = SYSCLK / Ticks
        proj.sim_hall_sensor_freq_read = DSP_CLOCK / proj.captured_ticks;
        
        // Convert to Mechanical RPM
        proj.w_m_rpm = proj.calc_simulated_w_m_feedback(proj.sim_hall_sensor_freq_read);
    } else {
        proj.w_m_rpm = 0;
        proj.sim_hall_sensor_freq_read = 0;
    }
    proj.results.w_m_calc = proj.w_m_rpm;
    

    // 3. Clear the eCAP module's internal interrupt flags
    proj.ecap.clear_interrupt_flag(1);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

void main(void)
{
    set_interrupt(&PieVectTable.SCIA_RX_INT, receiveModbusData, M_INT9, &PieCtrlRegs.PIEIER9.all, (1 << 0));
    set_interrupt(&PieVectTable.TIMER0_INT, timer_isr, M_INT1, &PieCtrlRegs.PIEIER1.all, (1 << 6));    

    proj.pwm.set_interrupt(simulateHallSensorPulses, 2);
    proj.pwm.enable_interrupt();
    proj.setup_pwm();
    proj.ecap.set_interrupt(getWmFromHallSensorPulses, 1);
    proj.setup_ecap();
    proj.ecap.enable_interrupt();
    proj.setup_mb();

    EINT;
    ERTM;

    proj.loop();

}



