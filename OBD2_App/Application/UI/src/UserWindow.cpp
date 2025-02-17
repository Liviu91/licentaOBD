// OBD2_App/Application/Frontend/UI/src/UserWindow.cpp
#include "UserWindow.h"

#include <imgui.h>
#include <iostream> //For printing debug messages
// other includes, for example for datalogging and communication if needed in this window


UserWindow::UserWindow() : isConnected(false), isStreaming(false), isLogging(false), isTechnicianRequestPending(false)
{


}

void UserWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}



void UserWindow::Draw() {
    ImGui::Begin("User Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);


    if (ImGui::BeginMenu("Connect")) {
        if (!isConnected) {
            if (ImGui::MenuItem("Connect to ELM327")) {
                // Handle connect

                 // ... (Your connection logic, potentially calling a ConnectionManager or similar)
                  // ... (Set isConnected = true on successful connection) ...

            }
        }
        else {
            if (ImGui::MenuItem("Disconnect from ELM327")) {
                // Handle disconnect

                // ... (Your disconnection logic)
                 //Set isConnected = false on disconnect


            }
        }
        ImGui::EndMenu();

    }

    if (isConnected) //Only display these menus if connected
    {



        if (ImGui::BeginMenu("Real-time Data")) {
            if (!isStreaming) {
                if (ImGui::MenuItem("Start Streaming"))
                {



                    isStreaming = true;

                    // ... (Start streaming logic.  Get the access key from the server, if applicable)
                }



            }
            if (isStreaming) {
                if (ImGui::MenuItem("Stop Streaming")) {
                    // ... (Stop streaming logic) ...

                    isStreaming = false;

                }
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("DTC")) {
            if (ImGui::MenuItem("View DTCs")) {
                // ... (View DTCs logic)

            }

            if (ImGui::MenuItem("Clear DTCs"))
            {
                // ... (Clear DTCs logic, with confirmation dialog if necessary)
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Data Logging"))
        {


            if (!isLogging)
            {




                if (ImGui::MenuItem("Start Logging"))
                {

                    //Handle start logging


                    isLogging = true;


                }





            }



            if (isLogging)
            {






                if (ImGui::MenuItem("Stop Logging"))
                {




                    isLogging = false;
                    //Handle stop logging



                }



            }


            ImGui::EndMenu();

        }




        if (ImGui::BeginMenu("Technician Request")) {

            if (isTechnicianRequestPending) //Only show this if there is a pending request
            {




                if (ImGui::MenuItem("Approve Request"))
                {
                    isTechnicianRequestPending = false;

                    // ... (Approve technician communication request logic)
                }


                if (ImGui::MenuItem("Deny Request"))
                {


                    isTechnicianRequestPending = false;
                    // ... (Deny technician communication request logic) ...

                }

            }


            ImGui::EndMenu();
        }






    }



    ImGui::End();


}
