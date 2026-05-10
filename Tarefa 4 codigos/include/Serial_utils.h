//
// Created by vibar on 05/05/2026.
//

#ifndef C_FOR_SIM_LIB_SERIAL_UTILS_H
#define C_FOR_SIM_LIB_SERIAL_UTILS_H
int openSerial(const char *portName);
void writeSerial(const char *data);
int readSerial(char *buffer, int bufSize);
void closeSerial();
#endif //C_FOR_SIM_LIB_SERIAL_UTILS_H