#include "MainWindow.h"
#include "PressConnect.h"
#include <iostream> //For std::cout and std::cerr
#include <Bluetooth_operations.h>
#include <Serial_port_connection.h>
#include <Communication_interfaces.h>
#include <PressConnect.h>
#include <imgui.h> // For ImGui functions and types



MainWindow::MainWindow() : window(nullptr)// Initialize window to nullptr
{
    isConnected = false;
    elm327Version = "";
    ecuId = "";
    communication_protocol = "";


}


void MainWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}


bool MainWindow::LoadFonts()
{


    ImGuiIO& io = ImGui::GetIO();
   defaultFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\times.ttf", 22.0f); //Change path if needed
   titleFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\times.ttf", 30.0f); //Change path if needed



    if (defaultFont == nullptr || titleFont == nullptr)
    {
        return false; //Return false on font load fail. The app should handle it.

    }

    return true; //Fonts are loaded correctly
}


//PressConnect::isConnected(false);

void MainWindow::Draw()
{
    //PressConnect pressConnect = false;
    bool pressConnectOneTime = false;
    //PressConnect pressConnect;
    ImGui::Begin("MainWindow");
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(1280, 720));


    // Title (Times New Roman, 18pt)
    ImGui::PushFont(titleFont); // Assuming you've loaded the title font
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("OBD-II Diagnosis Application").x) * 0.5f); // Centered
    ImGui::Text("OBD-II Diagnosis Application");
    ImGui::PopFont();



    ImGui::PushFont(defaultFont); // Assuming you've loaded the default font for rest of elements
    // Message Area (Left side)
    ImGui::BeginGroup();
    ImGui::Text("ELM327 version: %s", elm327Version.c_str());
    ImGui::Text("ECU id: %s", ecuId.c_str());
    ImGui::Text("VIN: %s", communication_protocol.c_str());
    ImGui::EndGroup();


    ImGui::PopFont();

    // Button Positioning
    float buttonWidth = 150.0f;  // Adjust as needed
    float buttonHeight = 30.0f; // Adjust as needed
    float windowWidth = ImGui::GetWindowWidth();
    float windowHeight = ImGui::GetWindowHeight();
    float verticalStart = windowHeight * (2.0f / 3.0f) - buttonHeight / 2; //Start at 2/3 for each side. Center button vertically by /2 its height


    //Connect button
    if (isConnected)
    {

        ImGui::BeginDisabled(); // Disable if already connected
        ImGui::Button("Connect", ImVec2(150, 50));
        ImGui::EndDisabled();


    }
    else if (ImGui::Button("Connect", ImVec2(150, 50)) && !pressConnectOneTime) //Only allow one click if not connected
    {


        pressConnectOneTime = true; // Disable the button


        pressConnect = std::make_unique<PressConnect>();

        if (pressConnect->Connect()) {
            isConnected = true;


            elm327Version = pressConnect->GetElm327Version();
            ecuId = pressConnect->GetEcuId();
            communication_protocol = pressConnect->GetCurrentProtocol();
        }
        else
        {
            //connectionMessage = pressConnect->GetLastErrorMessage();
        }

    }
    else if (!isConnected)
    {

        ImGui::BeginDisabled();
        ImGui::Button("Connect", ImVec2(150, 50));
        ImGui::EndDisabled();
    }

    // Left-side buttons
    ImGui::SetCursorScreenPos(ImVec2(10, verticalStart));   // Small margin from left. Set start position
    if (isConnected) {
        if (ImGui::Button("RealTime Data", ImVec2(buttonWidth, buttonHeight))) {
            // Handle RealTimeData button click
        }
    }
    else
    {
        ImGui::BeginDisabled();
        ImGui::Button("RealTime Data", ImVec2(buttonWidth, buttonHeight));
        ImGui::EndDisabled();


    }
    ImGui::SetCursorScreenPos(ImVec2(10, verticalStart + buttonHeight + 10)); // Place DTC button below RealTimeData with some spacing
    if (isConnected) {
        if (ImGui::Button("DTC", ImVec2(buttonWidth, buttonHeight))) {
            // Handle DTC button click
        }
    }
    else {
        ImGui::BeginDisabled();
        ImGui::Button("DTC", ImVec2(buttonWidth, buttonHeight));
        ImGui::EndDisabled();
    }





    // Right-side buttons
    ImGui::SetCursorScreenPos(ImVec2(windowWidth - buttonWidth - 10, verticalStart));  // Small margin from right
    if (isConnected) {

        if (ImGui::Button("Communicate with Elm327", ImVec2(buttonWidth, buttonHeight))) {
            // Handle Communicate button click
        }

    }
    else
    {
        ImGui::BeginDisabled();
        ImGui::Button("Communicate with Elm327", ImVec2(buttonWidth, buttonHeight));

        ImGui::EndDisabled();

    }

    ImGui::SetCursorScreenPos(ImVec2(windowWidth - buttonWidth - 10, verticalStart + buttonHeight + 10)); //Spacing for data logging button


    if (isConnected)
    {

        if (ImGui::Button("Data logging", ImVec2(buttonWidth, buttonHeight))) {

        }

    }
    else
    {

        ImGui::BeginDisabled();
        ImGui::Button("Data logging", ImVec2(buttonWidth, buttonHeight));

        ImGui::EndDisabled();

    }


    // Connect Button (Center-bottom)
    ImGui::SetCursorPosY(windowHeight - 60);


    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Connect").x) * 0.5f - 150 / 2);  // Center connect button


    ImGui::Text("%s", connectionMessage.c_str()); //Display messages regarding the connection



    ImGui::End();

}