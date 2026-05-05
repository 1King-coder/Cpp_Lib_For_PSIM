#include <math.h>
#include "../include/cpp_psim_lib.h"
#include <windows.h>
#include "../include/Serial_utils.h"


extern "C" {

    DLLEXPORT void SimulationBegin(
        const char *szId, int nInputCount, int nOutputCount,
         int nParameterCount, const char ** pszParameters,
         int *pnError, char * szErrorMsg,
         void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr) {
        openSerial("COM1");

        int bufSize = 1;

        char buf[bufSize];
        int index = 0;

        while (1) {
            char tempChar;
            int n = readSerial(&tempChar, 1);
            if (n > 0) {
                if (tempChar == '\n' || tempChar == '\r') {
                    if (index > 0) {
                        break;
                    }
                } else if (index < sizeof(buf)) {
                    char echoChar[2] = {tempChar, '\0'};
                    writeSerial(echoChar);
                    buf[index++] = tempChar;

                }
            } else {
                Sleep(10);
            }
        }
        writeSerial("\nReceived: ");
        writeSerial(buf);
        writeSerial("\n");
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
        *pnError = 0; //Success

    }

    DLLEXPORT void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
    {


    closeSerial();

    }




}



