#pragma once

#include <imgui.h>
#include <string>
#include <GLFW/glfw3.h>


class LoginWindow {
public:
    LoginWindow();

    void Draw();
    void Initialize(GLFWwindow* window);
    bool ShowLoginWindow();
    bool LoadFonts();
    bool IsLoggedIn() const;
    std::string GetUserRole() const;


private:
    bool showLoginWindow; //Flag for showing/hiding window


    bool loggedIn;  // Flag to track login status

    std::string userRole;  // To store the user's role ("user" or "technician")
    ImFont* defaultFont;  // Declare defaultFont as a member
    char usernameBuffer[256];
    char passwordBuffer[256];
    GLFWwindow* window;
    std::string loginMessage;


};
