#include <math.h>
#include "../include/cpp_psim_lib.h"
#include <windows.h>
#include "../include/Serial_utils.h"

#define COM_PORT "\\\\.\\COM4"
#include <chrono>
#include <stdint.h>

extern "C" {
    using namespace std;
    typedef chrono::milliseconds MSEC;
    typedef chrono::high_resolution_clock HRC;
    chrono::time_point<chrono::high_resolution_clock> start, time_end;

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
        start = HRC::now();
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

        x.u = (float) in[0];
        x.y = (float) out[0];
        x.gains.kp = (float) in[1];
        x.gains.ki = (float) in[2];

        writeSerial(&x, sizeof(x));
        int bytesReceived = 0;

        uint8_t * rxPtr = (uint8_t*) &y;

        while (bytesReceived < sizeof(y)) {
            bytesReceived += readSerial(rxPtr + bytesReceived, sizeof(y) - bytesReceived);
        }
        out[0] = (double) y;

        *pnError = 0; //Success

    }

    DLLEXPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
    {
        closeSerial();

        time_end = HRC::now();
        chrono::duration<float> duration = time_end - start; // calcula tempo total de execução
        float sec = duration.count();

        openSerial("\\\\.\\COM12"); // Abre porta COM para leitura do tempo

        char buffer[64];
        int len = snprintf(buffer, sizeof(buffer), "\n Time elapsed (s): %.3f\r\n", sec);

        writeSerial(buffer, len); // escreve o tempo de execução na porta serial
        closeSerial();
    }
}



