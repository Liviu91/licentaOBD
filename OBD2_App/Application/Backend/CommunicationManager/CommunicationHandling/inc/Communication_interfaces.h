#include <algorithm> // required for std::remove
#include <cctype>    // required for std::iscntrl
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
int SendELM327Command(HANDLE* hBluetoothPort, const std::string& command);
std::string ReadELM327Response(HANDLE* hComPort);
int InitializeELM327(HANDLE* hBluetoothPort);
std::string AskELM327(HANDLE* hBluetoothPort, const std::string& command);