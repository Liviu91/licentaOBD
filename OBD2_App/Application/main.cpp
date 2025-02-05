#include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Bluetooth_operations.h> 
#include <Serial_port_connection.h>
#include <Communication.h> 


int main(int argc, char** argv) 
{
    char exitKey;
    HANDLE hBluetoothSerialPort;
    std::string elm327Command;
    int bluetooth_connection_status = 0;
    int serial_port_setup_status = 0;

    if (!Connect_With_ELM327_via_Bluetooth()) {
        std::cout << "Error setting up bluetooth connection with ELM327 device" << std::endl;
        std::cout << "Try again..." << std::endl;
    }
    else // Set up the serial port connection.
        if (!SetupSerialPort(&hBluetoothSerialPort)) {
            // Serial port setup was successful.
            std::cout << "Try again..." << std::endl;
        }
        else {
            /*Initialize ELM327:
             * version, vin, ecu id, at e0? maybe more info to showcase at the beggining of the mainWindow*/
            if (!InitializeELM327(&hBluetoothSerialPort)) {
                std::cout << "Init succesfull!" << std::endl;
            }
            else {
                std::cerr << "Init failed!" << std::endl;
                return -1;
            }
        }

   
    // Main command loop.
    while (true) {
        std::cout << "Enter ELM327 command (or 'exit' to quit): ";
        std::getline(std::cin, elm327Command); // Read input command.

        if (elm327Command == "exit") {
            break; // Exit the loop when the user enters "EXIT".
        }

        if (SendELM327Command(&hBluetoothSerialPort, elm327Command) == 0) { // Try to send a command.
            std::string elm327Response = ReadELM327Response(&hBluetoothSerialPort); // Read response from ELM327.

            if (!elm327Response.empty()) {
                std::cout << elm327Response << std::endl; 
            }
            else {
                std::cerr << "No response from ELM327." << std::endl;
            }
        }
    }

    cout << "\nPress any key to exit" << endl; 

    CloseBluetoothHandles();
    exitKey = _getch();

    return 0; 
}

