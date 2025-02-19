#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "imgui.h"
#include <vector>
#include <string>
#include <GLFW/glfw3.h>

class UserWindow {
public:

    UserWindow();
    void Draw();
    void Initialize(GLFWwindow* window);
    void AddMessage(const std::string& msg);

private:
	bool isConnected = false;
    bool isStreaming = false;
    bool isLogging = false;
    GLFWwindow* window;
    bool isTechnicianRequestPending = false;
    std::vector<std::string> messageLog;
    void ConnectToELM327();
    void DisconnectFromELM327();
    void StartStreaming();
    void StopStreaming();
    void ManageDTCs();
    void LogData();

};

#endif // USERWINDOW_H