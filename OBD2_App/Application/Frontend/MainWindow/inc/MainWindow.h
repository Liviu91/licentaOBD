#pragma once  // Include guard
#include "PressConnect.h"
#include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>  // Include for std::string
#include <imgui.h> // Include for ImGui
#include <GLFW/glfw3.h>

class MainWindow {
public:
    MainWindow();

    void Initialize(GLFWwindow* window);

    void Draw();

    bool LoadFonts();

private:


  //  bool isConnected;

    std::string elm327Version; //Make sure it's std::string
    std::string ecuId;
    std::string communication_protocol;
    std::string connectionMessage;
    bool isConnected;
    GLFWwindow* window;
    ImFont* defaultFont;  // Declare defaultFont as a member
    ImFont* titleFont; //Declare titleFont as a member

    std::unique_ptr<PressConnect> pressConnect;

};