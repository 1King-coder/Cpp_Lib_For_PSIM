
#include "F28x_Project.h"
#include "F2837xD_device.h"
#include "BLDCDriveAndControlProject.h"
#include "sci_com.h"
#include "global_variables.h"

void set_interrupt (volatile PINT *pieVectorDest, PINT newIsr, 
                                                    Uint16 coreIerMask, volatile Uint16 *pieIerReg, Uint16 pieIerMask) 
{
    EALLOW;
    *pieVectorDest = newIsr;
    EDIS;

    *pieIerReg |= pieIerMask;

    IER |= coreIerMask;
}


__interrupt void processDataReceiveINT (void);

project::BLDCDriveAndControlProject proj;
struct project::BLDCDriveAndControlProject::BLDC_input_data x;
float y;


__interrupt void processDataReceiveINT (void) {
    receiveData(&x, 12);
    proj.w_m_pi_control(&x, &y);
    sendData(&y, 4);
    
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

void main(void)
{
    
    set_interrupt(&PieVectTable.SCIA_RX_INT, &processDataReceiveINT, M_INT9, &PieCtrlRegs.PIEIER9.all, (1 << 0));
    EINT;
    proj.loop();
    
}



