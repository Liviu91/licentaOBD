#include <Bluetooth_operations.h> 

int Connect_With_ELM327_via_Bluetooth() {

    char exitKey;
    DWORD serviceStateResult;
    bool deviceFoundResult;
    int bluetoothEnabledStatus; // Store Bluetooth status.
    BLUETOOTH_DEVICE_INFO targetBtDeviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO),0, }; // Device information structure.
    std::string elm327Command;

    bluetoothEnabledStatus = IsBluetoothEnabled(); // Verify if Bluetooth is activated.

    if (bluetoothEnabledStatus == 0) { // Bluetooth is deactivated.
        std::cout << "Bluetooth is disabled. Enable it and restart the application." << std::endl;
        std::cout << "Press any key to exit" << std::endl;
        exitKey = _getch();
        return -1;
    }
    else if (bluetoothEnabledStatus == -1) {
        std::cerr << "Error initializing Bluetooth. Exiting." << std::endl;
        std::cout << "Press any key to exit" << std::endl;
        exitKey = _getch();
        return -1;
    }

    // If Bluetooth is enabled, proceed with device discovery.
    deviceFoundResult = FindBluetoothDevice(&targetBtDeviceInfo);
    if (!deviceFoundResult) {
        // Device not found.
        // Device discovery was unsuccessful.
        std::cerr << "OBD-II adapter not detected." << std::endl;
        std::cout << "Retrying. Press any key to quit" << std::endl;
        while (!FindBluetoothDevice(&targetBtDeviceInfo)) { // Continue searching for the device.
            Sleep(1000);
            if (_getch()) { // Exit if any key is pressed.
                CloseBluetoothHandles();
                return -1;
            }
        }
    }

    if (!BluetoothIsConnectable(g_bluetoothRadio)) // Check if the radio accepts incoming connections.
    { // Attempt to enable incoming Bluetooth connections if disabled.
        std::cout << "Enabling Bluetooth incoming connections..." << std::endl;
        if (BluetoothEnableIncomingConnections(g_bluetoothRadio, TRUE))
            std::cout << "Succesfully enabled incoming connections" << std::endl;
        else
            std::cerr << "Failed to enable incoming connections" << std::endl;
    }
    else

        if (!targetBtDeviceInfo.fConnected && !targetBtDeviceInfo.fRemembered) { // Device is remembered but not currently connected.
            std::cout << "Device is out of range or switched Off." << std::endl;
        }

    if (!targetBtDeviceInfo.fConnected && targetBtDeviceInfo.fRemembered) {
        std::cout << "OBD-II adapter paired but not connected. Check power and range." << std::endl;
    }
    else if (!targetBtDeviceInfo.fRemembered && !targetBtDeviceInfo.fConnected) {
        std::cout << "Connecting and pairing with OBD-II adapter..." << std::endl;

        if (!targetBtDeviceInfo.fAuthenticated) { // Attempt authentication if not already done.
            BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Retrieve device info.
            if (!PairBluetoothDevice(targetBtDeviceInfo)) { // Pair with the device.
                std::cerr << "Pairing failed. Try again" << std::endl;
                CloseBluetoothHandles();
                return -1;
            }
        }

        serviceStateResult = BluetoothSetServiceState(g_bluetoothRadio, &targetBtDeviceInfo, &SerialPortServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE); // Enable serial port service.
        if (serviceStateResult != ERROR_SUCCESS && serviceStateResult != E_INVALIDARG) {// Verify if the service was successfully enabled (excluding invalid arguments).
            if (serviceStateResult == ERROR_INVALID_PARAMETER) {
                std::cerr << "Invalid serial port service parameter." << std::endl;
            }
            else if (serviceStateResult == ERROR_SERVICE_DOES_NOT_EXIST) {
                std::cerr << "Serial port service unavailable." << std::endl;
            }
            else {
                std::cerr << "Failed to enable serial port service (Code: " << serviceStateResult << ")." << std::endl;
            }

            std::cout << "Press any key to exit" << std::endl;
            CloseBluetoothHandles();
            exitKey = _getch();
            return -1;
        }

        BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Update device information.
        BluetoothUpdateDeviceRecord(&targetBtDeviceInfo);

        std::cout << "Connection to OBD-II adapter established." << std::endl;
        std::cout << "Device Details:" << std::endl;
        std::cout << "Device Name: " << targetBtDeviceInfo.szName << std::endl;

        wprintf(L"  \tDevice Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", targetBtDeviceInfo.Address.rgBytes[5],
            targetBtDeviceInfo.Address.rgBytes[4], targetBtDeviceInfo.Address.rgBytes[3], targetBtDeviceInfo.Address.rgBytes[2],
            targetBtDeviceInfo.Address.rgBytes[1], targetBtDeviceInfo.Address.rgBytes[0]);
        wprintf(L"  \tDevice Connection: %s\r\n", targetBtDeviceInfo.fConnected ? L"true" : L"false");
        wprintf(L"  \tDevice Authentication: %s\r\n", targetBtDeviceInfo.fAuthenticated ? L"true" : L"false");
        wprintf(L"  \tDevice Remembered: %s\r\n", targetBtDeviceInfo.fRemembered ? L"true" : L"false");

        std::cout << "Operation Successful check if comport created" << std::endl;

        return 0;

    }
}