#include <Communication.h>

int AT_Z_Command(HANDLE* hBluetooth_port, const std::string& command) {

    //DWORD n, lasterror = 0x0;
    //DWORD bytesWritten;
    //DWORD dwBytesRead = 0x4;
    ////char atz_command[] = "AT Z\r"; // I've also tried "ATZ\r", "ATI\n", "ATZ\n"
    //std::string atz_command = "AT Z\r";
    //char atz_reply[20];


    //if (!WriteFile(*hBluetooth_port, atz_command.c_str(), atz_command.length(), &bytesWritten, NULL)) {
    //std:cerr << "WriteFile failed: " << GetLastError() << std:: endl;
    //    //sreturn -1;
    //}
    //else {
    //    cout << "Writing on port succesfull!" << endl;
    //    return 0;
    //}

    DWORD bytesWritten;
    std::string commandToSend = command + "\r"; // Add carriage return

    if (!WriteFile(*hBluetooth_port, commandToSend.c_str(), commandToSend.length(), &bytesWritten, NULL)) {
        std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
        return -1;
    }

    if (bytesWritten != commandToSend.length()) {
        std::cerr << "WriteFile incomplete" << std::endl;
        return -1;
    }

    return 0; // Return 0 on success
 }

std::string readFromCOM3(HANDLE* hComPort) {

    // Set timeouts (important to avoid indefinite blocking)
    DWORD timeoutMs = 1000;
    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout = MAXDWORD;  // Maximum time between two bytes
    timeouts.ReadTotalTimeoutConstant = timeoutMs; // Time for the entire read operation
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(*hComPort, &timeouts)) {
        std::cerr << "SetCommTimeouts failed: " << GetLastError() << std::endl;
        return ""; // Return empty string on error
    }

    char buffer[1024]; // Adjust buffer size as needed
    DWORD bytesRead;
    std::string response;

    do {
        if (!ReadFile(*hComPort, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (GetLastError() != ERROR_IO_PENDING) { // Ignore pending if using overlapped I/O
                std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
                return ""; // Return empty string on error
            }
        }

        if (bytesRead > 0) {

            buffer[bytesRead] = '\0'; // Null-terminate the received string

            response += buffer;
        }
    } while (bytesRead == sizeof(buffer) - 1); // Keep reading if the buffer filled up (unlikely but safer)

    //cout << buffer;
    printf("%s", buffer);
    //std::cerr << " " << buffer << std::endl;
    // Purge the port in case any extra characters are waiting
    PurgeComm(*hComPort, PURGE_RXCLEAR | PURGE_RXABORT);
     
    return response;
}