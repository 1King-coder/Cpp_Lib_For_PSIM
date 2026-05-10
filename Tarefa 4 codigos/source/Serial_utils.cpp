//
// Created by vibar on 05/05/2026.
//
#include <windows.h>

HANDLE hSerial = INVALID_HANDLE_VALUE;

int openSerial(const char *portName) {
    hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) return 0;

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) return 0;

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) return 0;
    return 1;
}

void writeSerial(const char *data) {
    if (hSerial == INVALID_HANDLE_VALUE) return;
    DWORD bytesWritten;
    WriteFile(hSerial, data, strlen(data), &bytesWritten, nullptr);
}

int readSerial(char *buffer, int bufSize) {
    if (hSerial == INVALID_HANDLE_VALUE) return 0;
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, bufSize, &bytesRead, nullptr))
        return bytesRead;
    return 0;
}

void closeSerial() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}