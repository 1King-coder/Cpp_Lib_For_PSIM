#include <math.h>
#include "../include/cpp_psim_lib.h"
#include <windows.h>
#include "../include/Serial_utils.h"

#define COM_PORT "\\\\.\\COM5"
#include <stdint.h>

extern "C" {

    typedef struct  {
        float kp;
        float ki;
    } PI_GAINS;

    struct SimData {
        float u;
        float y;
        PI_GAINS gains;
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
        SimData x;
        float y;
        int timeout = 0;

        x.u = (float) in[0];
        x.y = (float) out[0];
        x.gains.kp = (float) in[1];
        x.gains.ki = (float) in[2];

        writeSerial(&x, sizeof(x));
        int bytesReceived = 0;

        uint8_t * rxPtr = (uint8_t*) &y;

        while (bytesReceived < sizeof(y) && timeout < 100000) {
            bytesReceived += readSerial(rxPtr + bytesReceived, sizeof(y) - bytesReceived);
            if (bytesReceived == 0) {
                timeout++;
            }
        }
        out[0] = (double) y;

        *pnError = 0; //Success

    }

    DLLEXPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
    {
        closeSerial();
    }
}



