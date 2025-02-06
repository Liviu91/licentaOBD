#include <Communication.h>

// Send a command to the ELM327 device.
int SendELM327Command(HANDLE* hBluetoothPort, const std::string& command) {
    DWORD bytesWritten;
    std::string commandToSend = command + "\r"; // Add carriage return

    if (!WriteFile(*hBluetoothPort, commandToSend.c_str(), commandToSend.length(), &bytesWritten, NULL)) {
        std::cerr << "Error writing to the serial port (Code: " << GetLastError() << ")." << std::endl;
        return -1;
    }
     
    if (bytesWritten != commandToSend.length()) {
        std::cerr << "Incomplete data transfer to serial port." << std::endl;
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
                std::cerr << "Serial port read failed (Code: " << GetLastError() << ")." << std::endl;
                return "";  // Indicates an error
            }
        }
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate
            response += buffer;
        }
    } while (bytesRead == sizeof(buffer) - 1); // Continue if buffer filled (unlikely)


    // 1. Remove leading non-alphanumeric characters
    size_t firstAlphaNum = response.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    if (firstAlphaNum != std::string::npos) {
        response.erase(0, firstAlphaNum);
    }
    
    // Remove trailing carriage returns and '>'
    size_t found = response.find_last_not_of("\r>");

    if (found != std::string::npos) {
        response.erase(found + 1);
    }
    else {
        // Handle the case where the response is empty or only contains "\r" or ">"
        response = ""; // Or some other appropriate action.
    }    
    return response;
}

int InitializeELM327(HANDLE* hBluetoothPort) {

    std::string response;

    std::cout << "...Attempting to initialize ELM327..." << std::endl; 

    // Send "AT Z" command and display response
    std::cout << "Resetting ELM327 to get the version number... ";

    if (SendELM327Command(hBluetoothPort, "AT Z") != 0) {
        std::cerr << "Command failed!" << std::endl;
        return -1;
    }

    response = ReadELM327Response(hBluetoothPort);

    if (!response.empty()) {
        std::cout << "OK" << std::endl;
        std::cout << "Current version: "<< response <<"" << std::endl;
    }

    else {
        std::cerr << "Command failed! (No response)" << std::endl;
        return -1;
    }

    // Send "AT E0" command and check the response
    std::cout << "Instruct ELM327 to not echo back the command sent... ";

    if (SendELM327Command(hBluetoothPort, "AT E0") != 0) {
        std::cerr << "Command failed!" << std::endl;
        return -1;
    }

    response = ReadELM327Response(hBluetoothPort); // Read the response

    if (response.find("OK") == std::string::npos) { // Check for "OK"
        std::cerr << "Echo Off command failed! Response: " << response << std::endl;
        return -1;
    }

    std::cout << "OK" << std::endl;

    return 0;

}