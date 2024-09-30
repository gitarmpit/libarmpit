#include <windows.h>
#include <iostream>

bool WriteToSerial(HANDLE hSerial, const std::string &data) {
    DWORD bytesWritten;
    return WriteFile(hSerial, data.c_str(), data.size(), &bytesWritten, NULL) && (bytesWritten == data.size());
}

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

bool ReadByte(HANDLE hSerial, unsigned char *byte) {
    DWORD bytesRead;
    return ReadFile(hSerial, byte, sizeof(*byte), &bytesRead, NULL) && (bytesRead == sizeof(*byte));
}

int main() {
    // Open serial port
    HANDLE hSerial = CreateFile("COM7",
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening COM7: " << GetLastError() << std::endl;
        return 1;
    }

    // Set up the serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600; // Set baud rate
    dcbSerialParams.ByteSize = 8;         // Data bits = 8
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity   = NOPARITY;   // No parity

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port parameters: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Write string to serial port
    std::string dataToSend = "Hello, Serial!";
    if (WriteToSerial(hSerial, dataToSend)) {
        std::cout << "Sent: " << dataToSend << std::endl;
    } else {
        std::cerr << "Error writing string to serial port." << std::endl;
    }

    // Write a single byte to serial port
    unsigned char byteToSend = 0x01; // Example byte
    if (WriteByte(hSerial, byteToSend)) {
        std::cout << "Sent byte: " << static_cast<int>(byteToSend) << std::endl;
    } else {
        std::cerr << "Error writing byte to serial port." << std::endl;
    }

    // Read from serial port
    char receivedData[101]; // Buffer for received data (100 + null terminator)
    if (ReadFromSerial(hSerial, receivedData, 100)) {
        std::cout << "Received: " << receivedData << std::endl;
    } else {
        std::cerr << "Error reading from serial port." << std::endl;
    }

    unsigned char receivedByte;
    if (ReadByte(hSerial, &receivedByte)) {
        std::cout << "Received byte: " << static_cast<int>(receivedByte) << std::endl;
    } else {
        std::cerr << "Error reading byte from serial port." << std::endl;
    }

    // Close the serial port
    CloseHandle(hSerial);
    return 0;
}
