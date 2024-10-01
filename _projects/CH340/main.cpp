#include <windows.h>
#include <iostream>


bool WriteByte(HANDLE hSerial, unsigned char byte) {
    DWORD bytesWritten;
    return WriteFile(hSerial, &byte, sizeof(byte), &bytesWritten, NULL) && (bytesWritten == sizeof(byte));
}

bool ReadFromSerial(HANDLE hSerial, char *buffer, DWORD bytesToRead) {
    DWORD bytesRead;
    bool result = ReadFile(hSerial, buffer, bytesToRead, &bytesRead, NULL);
    buffer[bytesRead] = '\0'; // Null-terminate the string
    return result && (bytesRead > 0);
}

bool ReadByte_(HANDLE hSerial, unsigned char *byte) {
    DWORD bytesRead;
    return ReadFile(hSerial, byte, sizeof(*byte), &bytesRead, NULL) && (bytesRead == sizeof(*byte));
}

bool ReadByte(HANDLE hSerial, unsigned char& byte) {
    DWORD bytesRead;
    BOOL rc = ReadFile(hSerial, &byte, 1, &bytesRead, NULL);
    return rc;
}


static void testSend(HANDLE hSerial) {

    unsigned char b = 0x0; 
    while (true)
    {
        printf("sending: %d\n", b);
        if (!WriteByte(hSerial, b++))
        {
            std::cerr << "Error writing byte to serial port." << std::endl;
        }
        ::Sleep(1000);
    }

}

static void testReceive(HANDLE hSerial) {

    unsigned char b = 0;
    while (true)
    {
        if (!ReadByte(hSerial, b))
        {
            std::cerr << "Read error." << std::endl;
        }
        printf("received: %d\n", b);
    }

}


int main() {
    // Open serial port
    HANDLE hSerial = CreateFileA("COM8",
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening COM8: " << GetLastError() << std::endl;
        return 1;
    }

    // Set up the serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_300;
        00; // Set baud rate
    dcbSerialParams.ByteSize = 8;         // Data bits = 8
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity   = NOPARITY;   // No parity

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port parameters: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // testSend(hSerial);
    testReceive(hSerial);

    // Close the serial port
    CloseHandle(hSerial);
    return 0;
}
