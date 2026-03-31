#ifndef _CPP_PSIM_LIB_LIBRARY_H
#define _CPP_PSIM_LIB_LIBRARY_H


#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif
#include <cstdint>
extern "C" {
    void calcOut(double *in, double *out);

}

#endif // C_FOR_SIM_LIB_LIBRARY_H