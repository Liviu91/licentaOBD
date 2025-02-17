#pragma once

#include <imgui.h>
#include <string>



class LoginWindow {
public:
    LoginWindow();



    void Draw();
    bool ShowLoginWindow();
    bool IsLoggedIn() const;
    std::string GetUserRole() const;


private:
    bool showLoginWindow; //Flag for showing/hiding window


    bool loggedIn;  // Flag to track login status

    std::string userRole;  // To store the user's role ("user" or "technician")

    char usernameBuffer[256];
    char passwordBuffer[256];

    std::string loginMessage;


};
