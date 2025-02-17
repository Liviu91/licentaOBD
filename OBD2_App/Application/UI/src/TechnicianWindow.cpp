#include "TechnicianWindow.h"
#include <imgui.h>


void TechnicianWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}


void TechnicianWindow::Draw() {

    ImGui::Begin("Technician Window");

    //Add server connection status and a way to connect to server (button, menu etc.)
    if (!isConnectedToServer)
    {
        if (ImGui::MenuItem("Connect to Server"))
        {
            // Implement connect to server functionality
        }
    }
    else
    {


        if (ImGui::MenuItem("Disconnect from Server"))
        {


            //Implement disconnect from server
        }



    }

    if (isConnectedToServer)
    {


        if (!isViewingData)
        {




            if (ImGui::MenuItem("View Real-time Data"))
            {

                //Implement input for key logic and data displaying if correct key
            }


        }
        else
        {





            //Implement logic for data viewing



        }



        if (!isLogging) {

            if (ImGui::MenuItem("Start Logging"))
            {
                //Implement start logging

            }
        }
        if (isLogging)
        {
            if (ImGui::MenuItem("Stop Logging"))
            {

                //Implement stop logging
            }
        }
        if (ImGui::MenuItem("Communicate with ELM327"))
        {


            //Implement communicate logic
        }


    }


    ImGui::End();
}
