#include "MainWindow.hpp"

#include <iostream> //For std::cout and std::cerr
#include <Bluetooth_operations.h>
#include <Serial_port_connection.h>
#include <Communication.h>

#include <imgui.h> // For ImGui functions and types


MainWindow::MainWindow() : isConnected(false), window(nullptr) // Initialize window to nullptr
{

    elm327Version = "";
    ecuId = "";
    vinNumber = "";


}


void MainWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}

void MainWindow::Draw()
{
    /* test imgui*/
    ImGui::Begin("Test Window");
    ImGui::Text("Hello, ImGui!");
    ImGui::End();

    //ImGui::Begin("OBD-II Diagnosis Application", nullptr, ImGuiWindowFlags_NoCollapse); //No collapse added
    //ImGui::SetWindowPos(ImVec2(0, 0));//Set the main window in the upper left corner
    //ImGui::SetWindowSize(ImVec2(1280, 720));//Set the main window size

    //ImGui::Text("OBD-II Diagnosis Application");




    //ImGui::BeginGroup(); // Group for labels on the left


    //ImGui::Text("ELM327 version: %s", elm327Version.c_str());

    //ImGui::Text("ECU id: %s", ecuId.c_str());

    //ImGui::Text("VIN: %s", vinNumber.c_str());

    //ImGui::EndGroup();




    //ImGui::SameLine(ImGui::GetWindowWidth() / 2); //Start from the middle


    //ImGui::BeginGroup();// Group for the buttons on the right

    //if (isConnected)
    //{



    //    if (ImGui::Button("RealTime Data"))
    //    {

    //        //Implement show realtime data window
    //    }





    //    if (ImGui::Button("DTC"))
    //    {


    //        //Implement show dtc data window
    //    }

    //}
    //else
    //{
    //    ImGui::BeginDisabled();

    //    ImGui::Button("RealTime Data");




    //    ImGui::Button("DTC");
    //    ImGui::EndDisabled();

    //}







    //if (isConnected) {
    //    if (ImGui::Button("Communicate with Elm327")) {
    //        // Implement show communication window


    //    }


    //    if (ImGui::Button("Data logging")) {
    //        // Implement show data logging window

    //    }
    //}
    //else
    //{

    //    ImGui::BeginDisabled();

    //    ImGui::Button("Communicate with Elm327");



    //    ImGui::Button("Data logging");

    //    ImGui::EndDisabled();


    //}

    //ImGui::EndGroup();






    //if (ImGui::Button("Connect", ImVec2(150, 50)))
    //{


    //    BLUETOOTH_DEVICE_INFO targetBtDeviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO),0, }; // Device information structure.
    //    HANDLE hBluetoothSerialPort;
    //    int bluetoothEnabledStatus; // Store Bluetooth status.


    //    //This is the code from main.cpp for connection. Added in button callback
    //    bluetoothEnabledStatus = IsBluetoothEnabled(); // Verify if Bluetooth is activated.

    //    if (bluetoothEnabledStatus == 0) { // Bluetooth is deactivated.

    //        //Display error message
    //        connectionMessage = "Bluetooth is disabled. Enable it and restart the application.";

    //    }
    //    else if (bluetoothEnabledStatus == -1) {

    //        connectionMessage = "Error initializing Bluetooth.";

    //    }
    //    else if (bluetoothEnabledStatus == 1)
    //    {

    //        bool deviceFoundResult = FindBluetoothDevice(&targetBtDeviceInfo);

    //        if (deviceFoundResult)
    //        {
    //            if (!BluetoothIsConnectable(g_bluetoothRadio)) // Check if the radio accepts incoming connections.
    //            { // Attempt to enable incoming Bluetooth connections if disabled.

    //                if (BluetoothEnableIncomingConnections(g_bluetoothRadio, TRUE))
    //                {


    //                }
    //                else
    //                {


    //                }
    //            }



    //            if (!targetBtDeviceInfo.fConnected && !targetBtDeviceInfo.fRemembered) { // Device is remembered but not currently connected.



    //            }

    //            if (!targetBtDeviceInfo.fConnected && targetBtDeviceInfo.fRemembered) {




    //            }
    //            else if (!targetBtDeviceInfo.fRemembered && !targetBtDeviceInfo.fConnected)
    //            {



    //                if (!targetBtDeviceInfo.fAuthenticated) { // Attempt authentication if not already done.
    //                    BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Retrieve device info.


    //                    if (!PairBluetoothDevice(targetBtDeviceInfo)) { // Pair with the device.


    //                        CloseBluetoothHandles();

    //                    }
    //                }


    //                DWORD serviceStateResult;
    //                serviceStateResult = BluetoothSetServiceState(g_bluetoothRadio, &targetBtDeviceInfo, &SerialPortServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE); // Enable serial port service.

    //                if (serviceStateResult != ERROR_SUCCESS && serviceStateResult != E_INVALIDARG) {// Verify if the service was successfully enabled (excluding invalid arguments).
    //                    if (serviceStateResult == ERROR_INVALID_PARAMETER) {

    //                    }
    //                    else if (serviceStateResult == ERROR_SERVICE_DOES_NOT_EXIST) {

    //                    }
    //                    else {


    //                    }




    //                    CloseBluetoothHandles();

    //                }
    //                BluetoothGetDeviceInfo(g_bluetoothRadio, &targetBtDeviceInfo); // Update device information.
    //                BluetoothUpdateDeviceRecord(&targetBtDeviceInfo);






    //            }



    //            if (SetupSerialPort(&hBluetoothSerialPort) == 0) {

    //                if (InitializeELM327(&hBluetoothSerialPort) == 0)
    //                {
    //                    isConnected = true; // Set connection status to true on success


    //                    std::string response;
    //                    response = ReadELM327Response(&hBluetoothSerialPort);
    //                    elm327Version = response; //Assign the response to the member variable for displaying on main window


    //                    //Get the ECU id and VIN number and assign the values to the variables

    //                    SendELM327Command(&hBluetoothSerialPort, "0902"); //Send the command for seed
    //                    response = ReadELM327Response(&hBluetoothSerialPort);


    //                    SendELM327Command(&hBluetoothSerialPort, "2101"); //Send the command for ecu
    //                    response = ReadELM327Response(&hBluetoothSerialPort);
    //                    ecuId = response;


    //                    SendELM327Command(&hBluetoothSerialPort, "0902"); //Send the command for vin
    //                    response = ReadELM327Response(&hBluetoothSerialPort);
    //                    vinNumber = response;


    //                }
    //                else {

    //                }

    //            }
    //            else {

    //            }



    //        }
    //        else
    //        {
    //            connectionMessage = "Device not found.";
    //        }

    //    }

    //} //End of connect button code



    //ImGui::Text("%s", connectionMessage.c_str()); //Display messages regarding the connection



    //ImGui::End();

}