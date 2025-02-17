#pragma once
#include <imgui.h>
#include <GLFW/glfw3.h>

class UserWindow {
public:

    UserWindow();

    void Draw();
    void Initialize(GLFWwindow* window);

    // Add setters for flags (isConnected, isStreaming, isLogging, isTechnicianRequestPending)

    void SetIsConnected(bool connected) { isConnected = connected; }
    void SetIsStreaming(bool streaming) { isStreaming = streaming; }
    void SetIsLogging(bool logging) { isLogging = logging; }

    void SetIsTechnicianRequestPending(bool pending) { isTechnicianRequestPending = pending; }



private:
    // Flags to control UI state (make these private)

    bool isConnected = false;
    bool isStreaming = false;
    bool isLogging = false;
    GLFWwindow* window;
    bool isTechnicianRequestPending = false;



};
