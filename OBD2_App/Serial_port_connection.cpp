#include <Serial_port_connection.h>


// Set up the serial port for communication.
int SetupSerialPort(HANDLE* hBluetoothPort) {
    std::wstring comPortName = L"\\\\.\\COM3"; // COM port for Bluetooth
    DCB serialPortParams = { 0 };
    serialPortParams.DCBlength = sizeof(serialPortParams);
    DWORD timeoutMilliseconds = 1000;
    COMMTIMEOUTS timeouts = { 0 };

    *hBluetoothPort = CreateFile(
        comPortName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (*hBluetoothPort == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {		
            std::cerr << "Serial port does not exist" << std::endl;
            return -1;
        }
        std::cerr << "Other errors occurred. Exiting program." << std::endl;
        return -1;
    }     
		// Get current port settings
		if (!GetCommState(*hBluetoothPort, &serialPortParams)) {
        std::cerr << "GetCommState failed/Baudrates can't be set!" << GetLastError() << std::endl;
        CloseHandle(*hBluetoothPort);
        return -1;
    }

    // Set baud rate, data bits, parity, etc.  (Example: 38400, 8N1)
    serialPortParams.BaudRate = CBR_38400;  // Set your desired baud rate
    serialPortParams.ByteSize = 8;
    serialPortParams.Parity = NOPARITY;
    serialPortParams.StopBits = ONESTOPBIT;

    // Apply the port settings
    if (!SetCommState(*hBluetoothPort, &serialPortParams)) {
        std::cerr << "Baudrates are NOT set: " << GetLastError() << std::endl;
        CloseHandle(*hBluetoothPort);
        return -1;
    }
    else {
        std::cout << "BaudrateS Set OK!" << std::endl;
    }
	
    // Set timeouts to avoid blocking indefinitely.  
    timeouts.ReadIntervalTimeout = MAXDWORD;  // Max time between bytes
    timeouts.ReadTotalTimeoutConstant = timeoutMilliseconds; // Timeout for the entire read operation
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(*hBluetoothPort, &timeouts)) {
        std::cerr << "SetCommTimeouts failed: " << GetLastError() << std::endl;
        CloseHandle(*hBluetoothPort);
        return -1;
    } else {
        std::cout << "Timeouts on port set OK" << std::endl;
    }

    return 0;
}