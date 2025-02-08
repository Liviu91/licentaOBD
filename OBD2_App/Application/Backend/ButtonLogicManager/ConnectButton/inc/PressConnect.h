// OBD2_App/Application/Backend/Communication/inc/PressConnect.h
#pragma once

#include <string> // For std::string

//Other includes if needed (for example windows.h if HANDLE is used here)


class PressConnect {
public:
    PressConnect();
    //PressConnect(const PressConnect&) = delete;  //Example if you want to block copy constructor
    //PressConnect& operator=(const PressConnect&) = delete; //Example if you want to block assignment operator, if you only want to move the object
    ~PressConnect();



    bool Connect();


    // Getters to access data from MainWindow
    const std::string& GetLastErrorMessage() const;
    const std::string& GetElm327Version() const;
    const std::string& GetEcuId() const;
    const std::string& GetVinNumber() const;

private:




    bool isConnected;




    HANDLE hBluetoothSerialPort;

    std::string elm327Version;
    std::string ecuId;
    std::string vinNumber;


    std::string lastErrorMessage;




};