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
    //PressConnect(const PressConnect&) = delete;  //Example if you want to block copy constructor
    //PressConnect& operator=(const PressConnect&) = delete; //Example if you want to block assignment operator, if you only want to move the object
    ~PressConnect();



    bool Connect();

    bool connectionOK;
    HANDLE hBluetoothSerialPort;
    // Getters to access data from MainWindow
 /*   const std::string& GetLastErrorMessage() const;
    const std::string& GetElm327Version() const;
    const std::string& GetEcuId() const;
    const std::string& GetVinNumber() const;*/

private:


   

    std::string elm327Version;
    std::string ecuId;
    std::string vinNumber;


    std::string lastErrorMessage;




};