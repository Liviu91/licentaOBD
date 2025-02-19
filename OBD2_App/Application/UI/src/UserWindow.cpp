#include "UserWindow.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

UserWindow::UserWindow() : isConnected(false), isStreaming(false), isLogging(false), isTechnicianRequestPending(false)
{


}

void UserWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}
void UserWindow::Draw() {

    ImGui::Begin("User Window", nullptr, ImGuiWindowFlags_MenuBar);

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
                    StartStreaming();
                }
                if (ImGui::MenuItem("Stop Stream")) {
                    StopStreaming();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("DTC")) {
                if (ImGui::MenuItem("Visualize DTCs")) {
                    ManageDTCs();
                }
                if (ImGui::MenuItem("Delete DTCs")) {
                    LogData();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Data Logging")) {
                if (ImGui::MenuItem("LogData")) {
                    ManageDTCs();
                }
                ImGui::EndMenu();
            }
            
        }
        ImGui::EndMenuBar();
    }

        ImGui::Separator();

        // Central message area
        ImGui::TextWrapped("Activity Log:");
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 150), true);
        for (const auto& msg : messageLog) {
            ImGui::TextWrapped("%s", msg.c_str());
        }
        ImGui::EndChild();

      ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}
void UserWindow::ConnectToELM327() {
    messageLog.push_back("Connecting to ELM327...");
    std::cout << "Connecting to ELM327..." << std::endl;
    // Actual connection logic here
}

void UserWindow::DisconnectFromELM327() {
    messageLog.push_back("Disconnecting from ELM327...");
    std::cout << "Disconnecting from ELM327..." << std::endl;
    // Actual disconnection logic here
}

void UserWindow::StartStreaming() {
    messageLog.push_back("Starting data stream...");
    std::cout << "Starting data stream..." << std::endl;
    // Streaming logic here
}

void UserWindow::StopStreaming() {
    messageLog.push_back("Stopping data stream...");
    std::cout << "Stopping data stream..." << std::endl;
    // Stopping stream logic here
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