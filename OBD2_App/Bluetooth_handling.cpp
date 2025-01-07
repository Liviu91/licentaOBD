#include <Bluetooth_handling.h> 
#pragma warning(disable: 4995)
HANDLE g_bluetoothRadio; // Global handle for Bluetooth radio.
HBLUETOOTH_AUTHENTICATION_REGISTRATION g_authenticationHandle = 0; // Global handle for Bluetooth Authentication.

// Checks if Bluetooth radio is active.
int IsBluetoothEnabled(void) {
    int lastError;
    HANDLE radioHandle;
    HBLUETOOTH_RADIO_FIND btRadioFindHandle;
    BLUETOOTH_FIND_RADIO_PARAMS btRadioSearchParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) }; // Parameters for Bluetooth radio search.
    int btStatus;
    
    // Attempt to find a Bluetooth radio.
    btRadioFindHandle = BluetoothFindFirstRadio(&btRadioSearchParams, &radioHandle);
    if (btRadioFindHandle != NULL) {
        std::cout << "Bluetooth Radio Found !" << std::endl;
        btStatus = 1;
        if (CloseHandle(radioHandle) == FALSE) {
            std::cout << "CloseHandle() failed with error code " << GetLastError() << std::endl;
            btStatus = -1;
        } // Close the radio handle.


    }
    else {
        lastError = GetLastError();
        btStatus = -1;
        if (lastError == ERROR_NO_MORE_ITEMS) {
            std::cout << "Bluetooth Module not enabled !" << std::endl;
            btStatus = 0;
        }
        else

            std::cout << "Bluetooth Radio search failed with error code" << lastError << std::endl;
    }

    return btStatus;
}

// Search for a specific Bluetooth device.
bool FindBluetoothDevice(BLUETOOTH_DEVICE_INFO* btDeviceInfo) {
    
    HBLUETOOTH_RADIO_FIND btRadioFindHandle;
    HBLUETOOTH_DEVICE_FIND btDeviceFindHandle;
    
    int radioID;
    int deviceID;
    DWORD btInfoStatus;
    bool searchResult = false;

    
    BLUETOOTH_FIND_RADIO_PARAMS btRadioSearchParams = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    BLUETOOTH_RADIO_INFO btRadioInfo = { sizeof(BLUETOOTH_RADIO_INFO),0, };
    BLUETOOTH_DEVICE_SEARCH_PARAMS deviceSearchParams = {
    sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
     1,
     0,
     1,
     1,
     1,
     15,
     NULL };
    BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO),0, };
    
    // Start Bluetooth radio search.
    btRadioFindHandle = BluetoothFindFirstRadio(&btRadioSearchParams, &g_bluetoothRadio);
    if (btRadioFindHandle == NULL) {
        std::cout << "Bluetooth Radio failed with error code" << GetLastError() << std::endl;
        searchResult = false;
    }

    radioID = 0;

    // Get radio info.   
    btInfoStatus = BluetoothGetRadioInfo(g_bluetoothRadio, &btRadioInfo);

    if (btInfoStatus == ERROR_SUCCESS)		std::cout << "Bluetooth Radio looks fine!" << std::endl;

    else {
        std::cout << "BluetoothGetRadioInfo() failed with error code " << btInfoStatus << std::endl;
        searchResult = false;
    }

    deviceSearchParams.hRadio = g_bluetoothRadio;
    ZeroMemory(&deviceInfo, sizeof(BLUETOOTH_DEVICE_INFO));
    deviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);



    // Start searching for Bluetooth devices for each radio.
    btDeviceFindHandle = BluetoothFindFirstDevice(&deviceSearchParams, &deviceInfo);

    if (btDeviceFindHandle != NULL)
        std::cout << "BluetoothFindFirstDevice() is working!" << std::endl;
    else {
        std::cout << "Failed to find Devices with error code:  " << GetLastError() << std::endl;
        searchResult = false;
    }

    radioID++;
    deviceID = 0;

    // Loop through discovered Bluetooth devices.

    do
    {
        // Print device information (optional).
        wprintf(L"\n\tDevice %d:\r\n", deviceID);
        wprintf(L"  \tInstance Name: %s\r\n", deviceInfo.szName);
        wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", deviceInfo.Address.rgBytes[5],
            deviceInfo.Address.rgBytes[4], deviceInfo.Address.rgBytes[3], deviceInfo.Address.rgBytes[2],
            deviceInfo.Address.rgBytes[1], deviceInfo.Address.rgBytes[0]);
        wprintf(L"  \tClass: 0x%08x\r\n", deviceInfo.ulClassofDevice);
        wprintf(L"  \tConnected: %s\r\n", deviceInfo.fConnected ? L"true" : L"false");
        wprintf(L"  \tAuthenticated: %s\r\n", deviceInfo.fAuthenticated ? L"true" : L"false");
        wprintf(L"  \tRemembered: %s\r\n", deviceInfo.fRemembered ? L"true" : L"false");

        deviceID++;


        if (wcscmp(deviceInfo.szName, L"OBDII") == 0) // Check if the device is named "OBDII".
        {
            *btDeviceInfo = deviceInfo;
            searchResult = true;
            break;
        }
        else {
            searchResult = false;
        }


    } while (BluetoothFindNextDevice(btDeviceFindHandle, &deviceInfo));

    // Close the device search handle.

    if (BluetoothFindDeviceClose(btDeviceFindHandle) == TRUE)
        std::cout << "BluetoothFindDeviceClose(btDeviceFindHandle) is OK!" << std::endl;
    else {
        std::cout << "BluetoothFindDeviceClose(btDeviceFindHandle) failed with error code " << GetLastError() << std::endl;
        searchResult = false;
    }


    if (BluetoothFindRadioClose(btRadioFindHandle) == TRUE)
        std::cout << "BluetoothFindRadioClose(btRadioFindHandle) is OK!" << std::endl;
    else {
        std::cout << "BluetoothFindRadioClose(btRadioFindHandle) failed with error code " << GetLastError() << std::endl;
        searchResult = false;
    }
    return searchResult;
}


bool PairBluetoothDevice(BLUETOOTH_DEVICE_INFO btDeviceInfo) {

    DWORD lastError;
    bool pairingResult = false;

    PWSTR* passkey = new PWSTR[1];
    passkey[0] = PWSTR(L"1234"); // Default passkey (can be changed)
    lastError = BluetoothAuthenticateDevice(NULL, g_bluetoothRadio, &btDeviceInfo, *passkey, 4); // 4 is the length of the passkey


    switch (lastError)
    {
    case(ERROR_SUCCESS):
        std::cout << "Device paired successfully" << std::endl;
        pairingResult = true;
        break;
    case(ERROR_CANCELLED):
        std::cout << "Device pairing failed" << std::endl;
        pairingResult = false;
        break;
    case(ERROR_INVALID_PARAMETER):
        std::cout << "Invalid parameters" << std::endl;
        pairingResult = false;
        break;
    case(ERROR_NO_MORE_ITEMS):
        std::cout << "Device not available" << std::endl;
        pairingResult = false;
        break;
    }

    if (lastError != ERROR_SUCCESS)
        std::cout << "Failure due to: " << GetLastError() << std::endl;
    return pairingResult;
}

// Closes Bluetooth handles and unregisters authentication.
void CloseBluetoothHandles(void) {

    if (CloseHandle(g_bluetoothRadio) == FALSE) {
        std::cout << "CloseHandle() failed with error code " << GetLastError() << std::endl;
    }
    BluetoothUnregisterAuthentication(g_authenticationHandle);

}