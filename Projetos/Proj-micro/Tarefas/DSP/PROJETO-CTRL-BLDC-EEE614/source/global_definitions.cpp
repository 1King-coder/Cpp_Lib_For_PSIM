#include "global_definitions.h"

void set_interrupt (volatile PINT *pieVectorDest, PINT newIsr, 
                    Uint16 coreIerMask, volatile Uint16 *pieIerReg, Uint16 pieIerMask) 
{
    EALLOW;
    *pieVectorDest = newIsr;
    EDIS;

    *pieIerReg |= pieIerMask;

    IER |= coreIerMask;
}

float powi(	float base, unsigned short exponent)
{
    float power = 1;

    for (unsigned short i = 0; i < exponent; ++i)
    {
        power *= base;
    }

    return power;
}

