#include <Bluetooth_interfaces.h> 
#include <UserWindow.h>

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
        std::cout << "Bluetooth adapter found!." << std::endl;
        btStatus = 1;
        if (!CloseHandle(radioHandle)) {
            std::cerr << "Failed to close Bluetooth handle (Code: " << GetLastError() << ")." << std::endl;
            btStatus = -1;
        } // Close the radio handle.


    }
    else {
        lastError = GetLastError();
        btStatus = -1;
        if (lastError == ERROR_NO_MORE_ITEMS) {
            std::cerr << "Bluetooth is not enabled !" << std::endl;
            btStatus = 0;
        } else {
            std::cerr << "Failed to find Bluetooth adapter (Code: " << lastError << ")." << std::endl;
        }
    }

    return btStatus;
}

// Search for a specific Bluetooth device.
bool FindBluetoothDevice(BLUETOOTH_DEVICE_INFO* btDeviceInfo) {
    
    HBLUETOOTH_RADIO_FIND btRadioFindHandle;
    HBLUETOOTH_DEVICE_FIND btDeviceFindHandle;
    
    UserWindow userWindow;

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
        std::cerr << "Failed to access Bluetooth adapter (Code: " << GetLastError() << ")." << std::endl;
        searchResult = false;
    }

    radioID = 0;

    // Get radio info.   
    btInfoStatus = BluetoothGetRadioInfo(g_bluetoothRadio, &btRadioInfo);

    if (btInfoStatus == ERROR_SUCCESS)		
        userWindow.AddMessage("Bluetooth Radio looks fine!");

    else {
        std::cerr << "Failed to get Bluetooth adapter information (Code: " << btInfoStatus << ")." << std::endl;
        searchResult = false;
    }

    deviceSearchParams.hRadio = g_bluetoothRadio;
    ZeroMemory(&deviceInfo, sizeof(BLUETOOTH_DEVICE_INFO));
    deviceInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);


    // Start searching for Bluetooth devices for each radio.
    btDeviceFindHandle = BluetoothFindFirstDevice(&deviceSearchParams, &deviceInfo);

    if (btDeviceFindHandle != NULL)
        std::cout << "Bluetooth search possible!" << std::endl;
    else {
        std::cerr << "Failed to start Bluetooth device search (Code: " << GetLastError() << ")." << std::endl;
        searchResult = false;
    }

    std::cout << "Start searching for Bluetooth devices..." << std::endl;
	radioID++;
    deviceID = 0;

    // Loop through discovered Bluetooth devices.

    do
    {
        // Print device information (optional).
        wprintf(L"\n\tBluetooth Device %d:\r\n", deviceID);
        wprintf(L"  \tInstance Name: %s\r\n", deviceInfo.szName);
        wprintf(L"  \tDevice Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", deviceInfo.Address.rgBytes[5],
            deviceInfo.Address.rgBytes[4], deviceInfo.Address.rgBytes[3], deviceInfo.Address.rgBytes[2],
            deviceInfo.Address.rgBytes[1], deviceInfo.Address.rgBytes[0]);
        wprintf(L"  \tDevice Class: 0x%08x\r\n", deviceInfo.ulClassofDevice);
        wprintf(L"  \tDevice Connection: %s\r\n", deviceInfo.fConnected ? L"true" : L"false");
        wprintf(L"  \tDevice Authentication: %s\r\n", deviceInfo.fAuthenticated ? L"true" : L"false");
        wprintf(L"  \tDevice Remembered: %s\r\n", deviceInfo.fRemembered ? L"true" : L"false");

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
        std::cout << "Device handle closed: Everything is OK!" << std::endl;
    else {
        std::cerr << "Failed to close device handle! (Code: " << GetLastError() << ")." << std::endl;
        searchResult = false;
    }

    if (BluetoothFindRadioClose(btRadioFindHandle) == TRUE)
        std::cout << "Bluetooth radio handle closed!" << std::endl;
    else {
        std::cerr << "Failed to close bluetooth radio handle! (Code: " << GetLastError() << ")." << std::endl;
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
            std::cout << "Device paired." << std::endl;
        pairingResult = true;
        break;
    case(ERROR_CANCELLED):
            std::cerr << "Pairing cancelled." << std::endl;
        pairingResult = false;
        break;
    case(ERROR_INVALID_PARAMETER):
            std::cerr << "Invalid pairing parameters." << std::endl;
        pairingResult = false;
        break;
    case(ERROR_NO_MORE_ITEMS):
            std::cerr << "Device unavailable for pairing." << std::endl;
        pairingResult = false;
        break;
        default: // Added a default case for more robust error handling
            std::cerr << "Pairing error: " << lastError << std::endl;

            break;
    }
    return pairingResult;
}

// Closes Bluetooth handles and unregisters authentication.
void CloseBluetoothHandles(void) {
    if (!CloseHandle(g_bluetoothRadio)) {
        std::cerr << "Failed to close Bluetooth radio handle (Code: " << GetLastError() << ")." << std::endl;
    }
    BluetoothUnregisterAuthentication(g_authenticationHandle);
}