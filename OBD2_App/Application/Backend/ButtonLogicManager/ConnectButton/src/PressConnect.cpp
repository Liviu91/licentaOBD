#include "PressConnect.h"

#include <Bluetooth_operations.h>
#include <Serial_port_connection.h>
#include <Communication_interfaces.h>

PressConnect::PressConnect() : connectionOK(false), hBluetoothSerialPort(NULL){

}


PressConnect::~PressConnect()
{
    // Close the serial port if it was successfully opened during the Connect function. Add other cleanup code if needed.  
    if (hBluetoothSerialPort != nullptr && hBluetoothSerialPort != INVALID_HANDLE_VALUE) {
        CloseHandle(hBluetoothSerialPort);
        hBluetoothSerialPort = nullptr; //Prevent potential double free
    }
    CloseBluetoothHandles();

}


bool PressConnect::Connect() {

    bool success = false;
    std::string command_version_number = "A@ 1";
    std::string command_current_protocol = "AT DP";
    //std::string command_vin_number = "09 02";
    std::string command_ecuid = "01 01";


    if (!connectionOK) // Check if already connected
    {

        if (Connect_With_ELM327_via_Bluetooth() && SetupSerialPort(&hBluetoothSerialPort) && InitializeELM327(&hBluetoothSerialPort))
        {
           connectionOK = true;  // Update connected status.  If all functions returned true, the connection is successful

           // Get initial values after successful connection          
           response_version_number = AskELM327(&hBluetoothSerialPort, command_version_number);
           response_current_protocol = AskELM327(&hBluetoothSerialPort, command_current_protocol);
           response_ecuid = AskELM327(&hBluetoothSerialPort, command_ecuid);

           success = true;
        }
        else
        {


           //Handle the error cases, based on last error message received
           success = false;


        }


    }
    else
    {
        success = true; //Or do nothing. Already connected


    }

    return success;


}


std::string PressConnect::GetElm327Version() {

    return response_version_number;
}

std::string PressConnect::GetCurrentProtocol() {

    return response_current_protocol;
}

std::string PressConnect::GetEcuId() {

    return response_ecuid;
}