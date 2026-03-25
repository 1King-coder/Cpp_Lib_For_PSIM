#ifndef _CPP_PSIM_LIB_LIBRARY_H
#define _CPP_PSIM_LIB_LIBRARY_H


#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif
#include <cstdint>
extern "C" {
    typedef struct {
        uint8_t Sa_top;
        uint8_t Sa_bot;
        uint8_t Sb_top;
        uint8_t Sb_bot;
        uint8_t Sc_top;
        uint8_t Sc_bot;
    } INVERSOR_SWITCHES;

    typedef struct {
        uint8_t HallA;
        uint8_t HallB;
        uint8_t HallC;
    } HALL_INPUTS;

    enum Rotation_Direction {
        CLOCKWISE,
        COUNTER_CLOCKWISE,
    };


    void bldcHallSensorComLogic (
        INVERSOR_SWITCHES* switches,
        HALL_INPUTS hallInputs,
        Rotation_Direction direction
    );

}

#endif // C_FOR_SIM_LIB_LIBRARY_H