#ifndef _CPP_PSIM_LIB_LIBRARY_H
#define _CPP_PSIM_LIB_LIBRARY_H


// #if BUILDING_DLL
#define DLLEXPORT __declspec(dllexport)
// #else
#define DLLIMPORT __declspec(dllimport)
// #endif
extern "C" {
    void calcOut(double *in, double *out);

}

#endif // C_FOR_SIM_LIB_LIBRARY_H