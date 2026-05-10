#include <math.h>
#include "../include/cpp_psim_lib.h"
#include <windows.h>
#include "../include/Serial_utils.h"

#define COM_PORT "\\\\.\\COM4"

extern "C" {

    DLLEXPORT void SimulationBegin(
        const char *szId, int nInputCount, int nOutputCount,
         int nParameterCount, const char ** pszParameters,
         int *pnError, char * szErrorMsg,
         void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr) {

        openSerial(COM_PORT); // Connecta na porta COM do DSP
    }

    DLLEXPORT void SimulationStep(
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
        out[0] = pow(in[0], 2);
        float x, y;

        x = (float) in[0];

        writeSerial(&x, sizeof(x));
        readSerial(&y, sizeof(y));

        out[0] = (double) y;

        *pnError = 0; //Success

    }

    DLLEXPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
    {
        closeSerial();
    }




}



