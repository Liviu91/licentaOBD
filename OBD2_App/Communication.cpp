#include <Communication.h>

// Send a command to the ELM327 device.
int SendELM327Command(HANDLE* hBluetoothPort, const std::string& command) {
    DWORD bytesWritten;
    std::string commandToSend = command + "\r"; // Add carriage return

    if (!WriteFile(*hBluetoothPort, commandToSend.c_str(), commandToSend.length(), &bytesWritten, NULL)) {
        std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
        return -1;
    }

    if (bytesWritten != commandToSend.length()) {
        std::cerr << "WriteFile incomplete" << std::endl;
        return -1;
    }

    return 0; // Success
 }

// Read the response from the ELM327 device.
std::string ReadELM327Response(HANDLE* hComPort) {
    char buffer[1024]; // Buffer for incoming data
    DWORD bytesRead;
    std::string response;

    do {
        if (!ReadFile(*hComPort, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (GetLastError() != ERROR_IO_PENDING) { // Ignore pending if overlapped I/O
                std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
                return "";  // indicates error
            }
        }
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate
            response += buffer;
        }
    } while (bytesRead == sizeof(buffer) - 1); // Continue if buffer filled (unlikely)

    PurgeComm(*hComPort, PURGE_RXCLEAR | PURGE_RXABORT);
     
    return response;
}