
#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "BLDCDriveAndControlProject.h"
#include "sci_com.h"
#include "global_definitions.h"


project::BLDCDriveAndControlProject proj;
Uint32 rxIsrCounter = 0;
Uint32 epwm1IsrCounter = 0;
Uint32 eCap1IsrCounter = 0;


__interrupt void processDataReceiveINT (void) {
    receiveData(&proj.circuit_data, 12);
    proj.w_m_pi_control(&proj.circuit_data, &proj.response.ref);
    sendData(&proj.response, 8);
    if (rxIsrCounter == 0) {
        proj.pwm.enable_pwm_output(1); 
    }
    

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
    rxIsrCounter++;
}

__interrupt void simulateHallSensorPulses (void) 
{
    proj.pwm.set_switching_frequency_Hz(1, proj.calc_simulated_hall_sensor_freq(proj.circuit_data.w_m_feedback));
    
    EPwm1Regs.TBPRD = proj.pwm.pwmTimeBasePeriod[0]; 

    proj.pwm.set_pwm_value(1, 0);
    
    EPwm2Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    
    epwm1IsrCounter++;
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
    proj.response.w_m_calc = proj.w_m_rpm;
    

    // 3. Clear the eCAP module's internal interrupt flags
    proj.ecap.clear_interrupt_flag(1);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    eCap1IsrCounter++;
}

void main(void)
{
    set_interrupt(&PieVectTable.SCIA_RX_INT, processDataReceiveINT, M_INT9, &PieCtrlRegs.PIEIER9.all, (1 << 0));
    proj.pwm.set_interrupt(simulateHallSensorPulses, 2);
    proj.setup_pwm();
    proj.pwm.enable_interrupt();
    proj.ecap.set_interrupt(getWmFromHallSensorPulses, 1);
    proj.setup_ecap();
    proj.ecap.enable_interrupt();
    EINT;
    ERTM;
    proj.loop();

}



