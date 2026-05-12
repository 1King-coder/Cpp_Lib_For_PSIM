#include <math.h>
#include "../include/cpp_psim_lib.h"
#include <windows.h>
#include "../include/Serial_utils.h"

#define COM_PORT "\\\\.\\COM5"
#include <stdint.h>

extern "C" {

    struct SimData {
        float v;
        float i;
        float p;
    };

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
        SimData x, y;

        x.v = (float) in[0];
        x.i = (float) in[1];

        writeSerial(&x, sizeof(x));

        int expectedBytes = sizeof(y);
        int totalRead = 0;

        uint8_t* rxPtr = (uint8_t*) &y;

        while (totalRead < expectedBytes) {
            int n = readSerial(rxPtr + totalRead, expectedBytes - totalRead);
            if (n > 0) {
                totalRead += n;
            } else {
                Sleep(1);
            }


        }

        out[0] = (double) y.p;

        *pnError = 0; //Success

    }

    DLLEXPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
    {
        closeSerial();
    }




}



