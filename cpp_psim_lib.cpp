#include <cstdint>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "cpp_psim_lib.h"

extern "C" {
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
        HALL_INPUTS hallInputs = {0, 0, 0};
        hallInputs.HallA = static_cast<uint8_t>(in[0]);
        hallInputs.HallB = static_cast<uint8_t>(in[1]);
        hallInputs.HallC = static_cast<uint8_t>(in[2]);

        INVERSOR_SWITCHES bldcSwitches = {
            0,0,0,0,0,0
        };

        bldcHallSensorComLogic(&bldcSwitches, hallInputs, CLOCKWISE);

        out[0] = bldcSwitches.Sa_top;
        out[1] = bldcSwitches.Sa_bot;
        out[2] = bldcSwitches.Sb_top;
        out[3] = bldcSwitches.Sb_bot;
        out[4] = bldcSwitches.Sc_top;
        out[5] = bldcSwitches.Sc_bot;


        *pnError = 0; //Success
    }

    void bldcHallSensorComLogic(
        INVERSOR_SWITCHES* switches,
        const HALL_INPUTS hallInputs,
        const Rotation_Direction direction
    ) {
        uint8_t hallIn = pow(2, 2) * hallInputs.HallA + pow(2, 1) * hallInputs.HallB + hallInputs.HallC;
        switch (direction) {

            case CLOCKWISE: {
                if (hallIn == 4) {
                    switches->Sa_top = 1;
                    switches->Sc_bot = 1;
                }
                if (hallIn == 6) {
                    switches->Sa_top = 1;
                    switches->Sb_bot = 1;
                }

                if (hallIn == 2) {
                    switches->Sb_top = 1;
                    switches->Sa_bot = 1;
                }
                if (hallIn == 3) {
                    switches->Sb_top = 1;
                    switches->Sc_bot = 1;
                }

                if (hallIn == 1) {
                    switches->Sc_top = 1;
                    switches->Sb_bot = 1;
                }
                if (hallIn == 5) {
                    switches->Sc_top = 1;
                    switches->Sa_bot = 1;
                }
                break;
            }


            case COUNTER_CLOCKWISE: {

                break;
            }

        }



    }

}



