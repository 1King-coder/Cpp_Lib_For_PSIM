#include <cstdint>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "cpp_psim_lib.h"

double y_1 = 0;
double y_2 = 0;
double x_1 = 0;
double x_2 = 0;


double B[3] = {
    9.8696e-06, 1.97392e-05, 9.8696e-06
};
double A[3] = {
    4.0062598696, -7.9999802608, 3.9937598696
};

extern "C" {
    void calcOut( double* in, double* out) {
        double x = *in;

        *out = (B[0] * x + B[1]*x_1 + B[2]*x_2 - A[1]*y_1 - A[2]*y_2)/A[0];
        y_2 = y_1;
        y_1 = *out;
        x_2 = x_1;
        x_1 = x;
    }

    void SimulationStep(
            double t,
            double delt,
            double* in,
            double* out,
            int* pnError,
            char* szErrorMsg,
            void** ptrUserData,
            int nThreadIndex,
            void * pAppPtr
    ) {
        calcOut(&in[0], &out[0]);
        *pnError = 0; //Success
    }



}



