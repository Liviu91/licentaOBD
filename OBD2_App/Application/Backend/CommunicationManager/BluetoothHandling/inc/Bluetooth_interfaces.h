#pragma once
#include <SDKDDKVer.h>
#include <windows.h>
#include <BluetoothAPIs.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <initguid.h>
#include <windef.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Bthprops.lib")

using namespace std;
// Define maximum name length for Bluetooth devices.
#define MAX_BLUETOOTH_DEVICE_NAME 248

// Declare global Bluetooth radio handle and authentication registration handle.
extern HANDLE g_bluetoothRadio;
extern HBLUETOOTH_AUTHENTICATION_REGISTRATION g_authenticationHandle;

// Check if the Bluetooth radio is enabled.
int IsBluetoothEnabled(void);

// Search for a specific Bluetooth device.
bool FindBluetoothDevice(BLUETOOTH_DEVICE_INFO* btDeviceInfo);

// Pair with a Bluetooth device.
bool PairBluetoothDevice(BLUETOOTH_DEVICE_INFO btDeviceInfo);

// Close Bluetooth handles and unregister authentication.
void CloseBluetoothHandles(void);