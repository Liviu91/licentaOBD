#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "imgui.h"
#include <vector>
#include <string>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include "UserPublish.h"

class UserWindow {
public:

    UserWindow();
    void Draw();
    void Initialize(GLFWwindow* window);
    void AddMessage(const std::string& msg);
    void ConnectToELM327();
    void StartLiveDataStreaming();
    void StopLiveDataStreaming();
    HANDLE hBluetoothSerialPort;
    UserPublish mqttPublisher;

private:
	bool isConnected = false;
    bool isStreaming = false;
    bool isLogging = false;
    GLFWwindow* window;
    bool isTechnicianRequestPending = false;
    std::vector<std::string> messageLog;
    std::thread streamingThread;
    std::thread connectionThread; // Background thread
<<<<<<< HEAD
    UserPublish mqttPublisher;
=======
   
>>>>>>> c3d9484 (MQTT connection exception: MQTT error [-1]: TCP/TLS connect failure)
    bool isConnecting =  false; // Ensures only one connection at a time
    void DisconnectFromELM327();
    void ManageDTCs();
    void LogData();

    int ParseELMResponse(const std::string& response, const std::string& type);
    void PublishLiveDataToMQTT(int rpm, int temp, int speed);

};

#endif // USERWINDOW_H