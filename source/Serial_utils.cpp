//
// Created by vibar on 05/05/2026.
//
#include <windows.h>

HANDLE hSerial = INVALID_HANDLE_VALUE;

int openSerial(const char *portName) {
    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) return 0;

    dcbSerialParams.BaudRate =  CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) return 0;

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(hSerial, &timeouts)) return 0;
    return 1;
}

void writeSerial(const void *data, int dataSize) {
    if (hSerial == INVALID_HANDLE_VALUE) return;
    DWORD bytesWritten;
    WriteFile(hSerial, data, dataSize, &bytesWritten, NULL);
}

int readSerial(void *buffer, int bufSize) {
    if (hSerial == INVALID_HANDLE_VALUE) return 0;
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, bufSize, &bytesRead, NULL))
        return bytesRead;
    return 0;
}

void closeSerial() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}