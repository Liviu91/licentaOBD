#include <Communication.h> 

HANDLE m_radio; //Windows handle to local radio, declared here as a global variable
HANDLE hBluetooth_port_operations;
HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle = 0;

int main(int argc, char** argv) 
{
    char exitKey;
    DWORD serviceStateResult;
    //const GUID SerialPortServiceClass_UU=ID = SerialPortServiceClass_UUID;
    bool deviceFoundResult;
    int bluetoothEnabledStatus; // Store Bluetooth status.
    BLUETOOTH_DEVICE_INFO targetBtDeviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO),0, }; // Device information structure.
    HANDLE hBluetoothSerialPort;
    std::string elm327Command;



    bluetoothEnabledStatus = IsBluetoothEnabled(); // Verify if Bluetooth is activated.

    if (bluetoothEnabledStatus == 0) { // Bluetooth is deactivated.
        cout << "Enable bluetooth Manually and restart program" << endl;
        cout << "Press any key to exit" << endl;
        exitKey = _getch();
        return 0;
    }
    else if (bluetoothEnabledStatus == -1) {
        cout << "Error Occured Closing Program" << endl;
        cout << "Press any key to exit" << endl;
        exitKey = _getch();
        return 0;
    }

    // If Bluetooth is enabled, proceed with device discovery.
    deviceFoundResult = FindBluetoothDevice(&targetBtDeviceInfo);
    if (deviceFoundResult == false)
    {// Device not found.
        // Device discovery was unsuccessful.
        cout << "Error cannot find device" << endl;
        cout << "Searching again, Press any key to exit program" << endl;
        while (FindBluetoothDevice(&targetBtDeviceInfo) == false) { // Continue searching for the device.
            Sleep(1000);
            if (_getch()) { // Exit if any key is pressed.
                CloseBluetoothHandles();
                return 0;
            }
        }
    }

    if (!BluetoothIsConnectable(g_bluetoothRadio)) // Check if the radio accepts incoming connections.
    { // Attempt to enable incoming Bluetooth connections if disabled.
        cout << "Incomming connection was not ON, turning it On" << endl;
        if (BluetoothEnableIncomingConnections(g_bluetoothRadio, TRUE))
            cout << "Incomming connections enabled" << endl;
        else
            cout << "Error Unable to enable incoming connections" << endl;
    }
    else
        cout << "Incomming connection was ON!" << endl;


    if (targetBtDeviceInfo.fConnected == FALSE && targetBtDeviceInfo.fRemembered == TRUE) { // Device is remembered but not currently connected.
        cout << "Device is out of range or switched Off." << endl;
    }

    if (targetBtDeviceInfo.fRemembered == FALSE && targetBtDeviceInfo.fConnected == FALSE) { // If the device is not remembered, attempt to pair and connect.
        cout << "Device Found Attempting to connect" << endl;

        if (targetBtDeviceInfo.fAuthenticated == FALSE) { // Attempt authentication if not already done.
            BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Retrieve device info.
            if (!PairBluetoothDevice(targetBtDeviceInfo)) { // Pair with the device.
                cout << "Authentication failed, Try manually" << endl;
                CloseBluetoothHandles();
                return 0;
            }
        } 

        serviceStateResult = BluetoothSetServiceState(g_bluetoothRadio, &targetBtDeviceInfo, &SerialPortServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE); // Enable serial port service.
        if (serviceStateResult != ERROR_SUCCESS && serviceStateResult != E_INVALIDARG) {// Verify if the service was successfully enabled (excluding invalid arguments).
            if (serviceStateResult == ERROR_INVALID_PARAMETER)
                cout << "Invalid Parameter" << endl;

            if (serviceStateResult == ERROR_SERVICE_DOES_NOT_EXIST)
                cout << "Service not found" << endl;

            cout << "Press any key to exit" << endl;
            CloseBluetoothHandles();
            exitKey = _getch();
            return 0;
        }

        BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Update device information.
        BluetoothUpdateDeviceRecord(&targetBtDeviceInfo);

        cout << "Name: " << targetBtDeviceInfo.szName << endl;

        wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", targetBtDeviceInfo.Address.rgBytes[5],

            targetBtDeviceInfo.Address.rgBytes[4], targetBtDeviceInfo.Address.rgBytes[3], targetBtDeviceInfo.Address.rgBytes[2],

            targetBtDeviceInfo.Address.rgBytes[1], targetBtDeviceInfo.Address.rgBytes[0]);
        wprintf(L"  \tConnected: %s\r\n", targetBtDeviceInfo.fConnected ? L"true" : L"false");
        wprintf(L"  \tAuthenticated: %s\r\n", targetBtDeviceInfo.fAuthenticated ? L"true" : L"false");
        wprintf(L"  \tRemembered: %s\r\n", targetBtDeviceInfo.fRemembered ? L"true" : L"false");

        cout << "Operation Successful check if comport created" << endl;
    }


    cout << "Opening created Bluetooth COM port and setting baudrates" << endl;
    
    // Set up the serial port connection.
    if (SetupSerialPort(&hBluetoothSerialPort) == 0) {
        // Serial port setup was successful.
    }
    else {
        cout << "Error setting up serial port. Closing application." << endl;
        return -1;
    }

    // Main command loop.
    while (true) {
        std::cout << "Enter ELM327 command (or 'EXIT' to quit): ";
        std::getline(std::cin, elm327Command); // Read input command.

        if (elm327Command == "EXIT") {
            break; // Exit the loop when the user enters "EXIT".
        }

        if (SendELM327Command(&hBluetoothSerialPort, elm327Command) == 0) { // Try to send a command.
            std::string elm327Response = ReadELM327Response(&hBluetoothSerialPort); // Read response from ELM327.

            if (!elm327Response.empty()) {
                std::cout << elm327Response << std::endl;
            }
            else {
                std::cerr << "No response from ELM327." << std::endl;
            }
        }
    }

    cout << "\nPress any key to exit" << endl; 

    CloseBluetoothHandles();
    exitKey = _getch();

    return 0; 
}

