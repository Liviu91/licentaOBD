#pragma once

#include <imgui.h>
#include <GLFW/glfw3.h>


class TechnicianWindow {
public:
    void Draw();

    void Initialize(GLFWwindow* window);
    void SetIsConnectedToServer(bool connected) { isConnectedToServer = connected; }

    void SetIsViewingData(bool viewing) { isViewingData = viewing; }

    void SetIsLogging(bool logging) { isLogging = logging; }



private:

    // Flags for UI state

    bool isConnectedToServer = false;
    GLFWwindow* window;
    bool isViewingData = false;
    bool isLogging = false;


};