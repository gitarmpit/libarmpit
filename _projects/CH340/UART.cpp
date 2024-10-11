#include "UART.h"
#include <stdio.h>
#include <chrono>

static std::string GetErrorMessage(DWORD errorCode) {
    LPVOID lpMsgBuf;
    std::string errorMessage;

    // Format the message
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPSTR)&lpMsgBuf,
        0,
        NULL
    );

    // Convert the message to a std::string
    errorMessage = static_cast<char*>(lpMsgBuf);

    // Free the buffer allocated by FormatMessage
    LocalFree(lpMsgBuf);

    if (errorMessage.length() > 2) {
        errorMessage.pop_back(); 
        errorMessage.pop_back();
    }

    return errorMessage;
}


UART::UART()
{
    _initialized = false;
    _hSerial = NULL;
}

bool UART::Initialize(const char* port, int baudRate)
{
    // Open serial port
    _hSerial = CreateFileA(port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (_hSerial == INVALID_HANDLE_VALUE) {
        auto err = GetLastError();
        printf("Error opening %s: 0x%x (%s)\n", port, err, GetErrorMessage(err).c_str());
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;         
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;  

    if (!SetCommState(_hSerial, &dcbSerialParams)) {
        auto err = GetLastError();
        printf("Error setting COM parameters: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
        CloseHandle(_hSerial);
        return false;
    }
    return true;
}

uint32_t UART::WaitRead(int timeoutMs)
{
    if (timeoutMs < 0)
    {
        timeoutMs = 0;
    }

    DWORD rxBytes = 0;
    COMSTAT comStat;

    auto start = std::chrono::steady_clock::now();
    while (true)
    {
        rxBytes = 0;
        if (ClearCommError(_hSerial, NULL, &comStat)) {
            rxBytes = comStat.cbInQue;
        }
        else {
            auto err = GetLastError();
            printf("COM error: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
            exit(1);
        }

        if (rxBytes != 0) {
            break;
        }

        if (timeoutMs != 0)
        {
            auto now = std::chrono::steady_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            if (dur.count() > timeoutMs)
            {
                break;
            }
        }

        Sleep(1);
    }

    return rxBytes;
}


bool UART::Read(uint8_t* buf, DWORD bytesToRead, int timeoutMs) {
    DWORD totalBytesRead = 0; // Total bytes read so far
    bool rc = false;
    //printf("reading %d bytes\n", bytesToRead);
    /*
    while (true) {
        COMSTAT comStat;
        if (ClearCommError(_hSerial, NULL, &comStat)) {
            printf("q: %d\n", comStat.cbInQue);
        }
        ::Sleep(200);
    }
    */

    while (totalBytesRead < bytesToRead) {
        DWORD rxBytes = WaitRead(timeoutMs);
        //printf("rxBytes: %d\n", rxBytes);
        if (rxBytes == 0) {
            break;
        }

        DWORD bytesToReadNow = min(rxBytes, bytesToRead - totalBytesRead);

        DWORD bytesRead = 0; 

        if (ReadFile(_hSerial, buf + totalBytesRead, bytesToReadNow, &bytesRead, NULL)) {
            totalBytesRead += bytesRead; 
            //printf("totalBytesRead: %d\n", totalBytesRead);
            if (bytesRead == 0) {
                break;
            }
        }
        else {
            auto err = GetLastError();
            printf("Read error: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
            exit(1);
        }
    }

    if (totalBytesRead == bytesToRead) {
        //printf("read ok\n");
        rc = true; 
    }

    return rc;
}

DWORD UART::Read(uint8_t* buf, int timeoutMs) {

    bool rc = false;
    DWORD totalBytesRead = 0; // Total bytes read so far

    DWORD rxBytes = 0;
    while (rxBytes = WaitRead(timeoutMs)) {

        DWORD bytesRead = 0;

        if (ReadFile(_hSerial, buf + totalBytesRead, rxBytes, &bytesRead, NULL)) {
            totalBytesRead += bytesRead;
            if (bytesRead == 0) {
                break;
            }
        }
        else {
            auto err = GetLastError();
            printf("Read error: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
            exit(1);
        }
    }

    return totalBytesRead;
}

DWORD UART::Read(uint8_t* buf, DWORD bytesToRead) {

    DWORD bytesRead = 0;
    if (!ReadFile(_hSerial, buf, bytesToRead, &bytesRead, NULL)) {
        auto err = GetLastError();
        printf("Read error: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
        exit(1);
    }

    return bytesRead;
}


bool UART::SetTimeout(int timeoutMs)
{
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = timeoutMs; // Max time between received characters
    //timeouts.ReadTotalTimeoutConstant = 50; // Total timeout for read operation
    //timeouts.ReadTotalTimeoutMultiplier = 10; // Multiplier for read timeout
    //timeouts.WriteTotalTimeoutConstant = 50; // Timeout for write operation
    //timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplier for write timeout

    bool rc = true;
    if (!SetCommTimeouts(_hSerial, &timeouts)) {
        auto err = GetLastError();
        printf("Error setting serial port timeout: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
        CloseHandle(_hSerial);
        rc = false;
    }
    return rc;
}


bool UART::Write(const uint8_t* buf, DWORD length) {
    DWORD bytesWritten;
    if (!WriteFile(_hSerial, buf, length, &bytesWritten, NULL)) {
        auto err = GetLastError();
        printf("Write error: 0x%x (%s)\n", err, GetErrorMessage(err).c_str());
        exit(1);
    }
    return true;
}