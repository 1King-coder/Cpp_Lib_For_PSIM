#include "control_logics.h"
#include "global_variables.h"

void PI (PI_CTS constants, float *y, float x, float *y1, float *x1, float T) {
    *y = constants.kp * (x - *x1) + constants.ki * T / 2 * (x + *x1) + *y1;
    if (*y > constants.hLim) {
        *y = constants.hLim;
    } else if (*y < constants.lLim) {
        *y = constants.lLim;
    }
    *y1 = *y;
    *x1 = x;
}