#include "PressConnect.h"

#include <Bluetooth_operations.h>
#include <Serial_port_connection.h>
#include <Communication_interfaces.h>

PressConnect::PressConnect() : connectionOK(false), hBluetoothSerialPort(NULL){};




bool PressConnect::Connect() {
    //MainWindow::PressConnect() : hBluetoothSerialPort = NULL;
    //HANDLE hBluetoothSerialPort = NULL;
    // Initialize return value in case all steps are skipped (e.g., already connected). This ensures that there is a value returned for every case.
    bool success = false;


    if (!connectionOK) // Check if already connected
    {

        if (Connect_With_ELM327_via_Bluetooth() && SetupSerialPort(&hBluetoothSerialPort) && InitializeELM327(&hBluetoothSerialPort))
        {
            connectionOK = true;  // Update connected status.  If all functions returned true, the connection is successful


            // Get initial values after successful connection
            std::string response = ReadELM327Response(&hBluetoothSerialPort);
            elm327Version = response;


            SendELM327Command(&hBluetoothSerialPort, "0902"); //Request seed for ECU and VIN
            response = ReadELM327Response(&hBluetoothSerialPort);


            if (response.find("49 02") != std::string::npos || response.find("4902") != std::string::npos) //Check for positive response from seed request
            {


                SendELM327Command(&hBluetoothSerialPort, "2101"); //Request ECU id
                response = ReadELM327Response(&hBluetoothSerialPort);
                ecuId = response;


                SendELM327Command(&hBluetoothSerialPort, "0902"); //Request VIN number
                response = ReadELM327Response(&hBluetoothSerialPort);
                vinNumber = response;


            }



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
