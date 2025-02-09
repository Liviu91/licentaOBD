// OBD2_App/Application/Backend/Communication/inc/PressConnect.h
#pragma once

#include <string> // For std::string
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
//Other includes if needed (for example windows.h if HANDLE is used here)


class PressConnect {
public:
    PressConnect();
    ~PressConnect();

    bool Connect();

    bool connectionOK;
    HANDLE hBluetoothSerialPort;
    // Getters to access data from MainWindow
    std::string GetLastErrorMessage();
    std::string GetElm327Version();
    std::string GetEcuId();
    std::string GetCurrentProtocol();

private:
   

    std::string response_version_number;
    std::string response_ecuid;
    std::string response_current_protocol;

    std::string lastErrorMessage;




};