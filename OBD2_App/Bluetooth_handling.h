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

#define MAX_NAME 248

typedef DWORD(WINAPI* f_BluetoothEnableRadio)(BOOL);
typedef int (WINAPI* f_BthSetMode)(enum RadioMode);
extern HANDLE m_radio; //Windows handle to local radio, declared here as a global variable
extern HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle;
/// Bluetooth states.
enum RadioMode //enumeration created for bluetooth radio state.
{
	/// Bluetooth off.
	Off,
	/// Bluetooth is on but not discoverable.
	On,
	/// Bluetooth is on and discoverable.
	Discoverable,
};

//================================function prototypes==================================================================================//
int IsBluetoothOn(void);//returns 0 if bluetooth radio off, returns 1 if On and returns -1 if the function failed to complete.
bool pairDevice(BLUETOOTH_DEVICE_INFO); //returns true if successful and false if not takes bluetooth deivce info struct.
bool FindBtDev(BLUETOOTH_DEVICE_INFO*); //returns true if successful and false if not takes bluetooth deivce info struct's pointer.
void CloseAllHandle(void);//a function to close all windows handles. wil be called at last as a clean up function.
//======================================================================================================================================
