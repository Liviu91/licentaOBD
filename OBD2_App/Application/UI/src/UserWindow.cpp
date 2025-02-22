#include "UserWindow.h"
#include "UserPublish.h"
#include "PressConnect.h"
#include "Bluetooth_operations.h"
#include "Serial_port_connection.h"
#include "Communication_interfaces.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

UserWindow::UserWindow() : isConnected(false), isStreaming(false), isLogging(false), isTechnicianRequestPending(false), hBluetoothSerialPort(NULL), mqttPublisher()
{
    
}

void UserWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}
void UserWindow::AddMessage(const std::string& msg) {
    messageLog.push_back(msg);
}
void UserWindow::ConnectToELM327() {
    if (isConnecting) return; // Prevent multiple simultaneous connections
    isConnecting = true;

    // Launch a separate thread for connection
    connectionThread = std::thread([this]() {
        AddMessage("Connecting to ELM327...");

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (1/*!Connect_With_ELM327_via_Bluetooth() && !SetupSerialPort(&hBluetoothSerialPort) && !InitializeELM327(&hBluetoothSerialPort)*/)
            isConnecting = false;
            isConnected = true;
        });
    connectionThread.detach(); // Allow it to run independently
}

void UserWindow::DisconnectFromELM327() {
    messageLog.push_back("Disconnecting from ELM327...");
    //std::cout << "Disconnecting from ELM327..." << std::endl;
    // Actual disconnection logic here
}

void UserWindow::ManageDTCs() {
    messageLog.push_back("Managing DTCs...");
    std::cout << "Managing DTCs..." << std::endl;
    // DTC management logic here
}

void UserWindow::LogData() {
    messageLog.push_back("Logging data...");
    std::cout << "Logging data..." << std::endl;
    // Data logging logic here
}
void UserWindow::StartLiveDataStreaming() {
    if (!isConnected) {
        AddMessage("Cannot start streaming: Not connected to ELM327.");
        return;
    }

    isStreaming = true;
    AddMessage("Starting live data stream...");

    streamingThread = std::thread([this]() {
        while (isStreaming) {
            std::string rpmResponse = "OK";//AskELM327(&hBluetoothSerialPort, "AT E0"); // Request Engine RPM
            std::string tempResponse = "OK";//AskELM327(&hBluetoothSerialPort, "AT E0"); // Request Engine Coolant Temp
            std::string speedResponse = "OK";//AskELM327(&hBluetoothSerialPort, "AT E0"); // Request Vehicle Speed

            if (!rpmResponse.empty() || !tempResponse.empty() || !speedResponse.empty()) {
                int rpm = 111;//ParseELMResponse(rpmResponse, "RPM");
                int temp = 222;//ParseELMResponse(tempResponse, "CoolantTemp");
                int speed = 333;///ParseELMResponse(speedResponse, "Speed");

                std::string message = "RPM: " + std::to_string(rpm) + " | Coolant Temp: " + std::to_string(temp) + "°C | Speed: " + std::to_string(speed) + " km/h";
                AddMessage(message);
                PublishLiveDataToMQTT(rpm, temp, speed);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Send data every second
        }
        });
}

void UserWindow::StopLiveDataStreaming() {
    isStreaming = false;
    AddMessage("Stopping live data stream...");
    if (streamingThread.joinable()) {
        streamingThread.join();
    }
}

int UserWindow::ParseELMResponse(const std::string& response, const std::string& type) {
    if (response.size() < 4) return -1; // Ensure valid response

    int A = std::stoi(response.substr(4, 2), nullptr, 16);
    if (type == "RPM") {
        int B = std::stoi(response.substr(6, 2), nullptr, 16);
        return ((A * 256) + B) / 4;
    }
    else if (type == "CoolantTemp") {
        return A - 40; // Convert to Celsius
    }
    else if (type == "Speed") {
        return A; // Speed in km/h
    }
    return -1;
}

void UserWindow::PublishLiveDataToMQTT(int rpm, int temp, int speed) {
    
    
    //const std::string caca = "caca";
    mqttPublisher.Publish("car/engine/rpm", std::to_string(rpm));
    mqttPublisher.Publish("car/engine/coolant_temp", std::to_string(temp));
    mqttPublisher.Publish("car/vehicle/speed", std::to_string(speed));
}

void UserWindow::Draw() {

    ImGui::SetNextWindowSize(ImVec2(500, 600));
    // User Window with Menu Bar Flag
    ImGui::Begin("User Window", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

    // Menu Bar inside the User Window
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::BeginMenu("Connect to ELM327")) {
                if (ImGui::MenuItem("Connect")) {
                    ConnectToELM327();
                }
                if (ImGui::MenuItem("Disconnect")) {
                    DisconnectFromELM327();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Start Streaming Live Data")) {
                if (ImGui::MenuItem("Start Stream")) {
                    StartLiveDataStreaming();
                }
                if (ImGui::MenuItem("Stop Stream")) {
                    StopLiveDataStreaming();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("See/Delete DTCs")) {
                ManageDTCs();
            }
            if (ImGui::MenuItem("Data Logging")) {
                LogData();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Separator();
    //// Centered Activity Log title
    //ImVec2 windowSize = ImGui::GetWindowSize();
    //float textWidth = ImGui::CalcTextSize("Activity Log").x;
    //ImGui::SetCursorPosX((windowSize.x - textWidth) * 0.5f);
    //ImGui::Text("Activity Log");
       // Centered User Window title
   
   // Centered Message Area with Size (250,350)
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPosX((availSize.x - 250) * 0.5f);
    ImGui::SetCursorPosY(((availSize.y - 350) * 0.5f) + 50);
    ImGui::BeginChild("ScrollingRegion", ImVec2(250, 350), true);
    for (const auto& msg : messageLog) {
        ImVec2 textSize = ImGui::CalcTextSize(msg.c_str());
        ImGui::SetCursorPosX((250 - textSize.x) * 0.5f); // Center messages
        ImGui::TextWrapped("%s", msg.c_str());
    }
    ImGui::EndChild();
    ImGui::End(); // Close "User Window"

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
