// OBD2_App/Application/Frontend/UI/src/LoginWindow.cpp
#include "LoginWindow.h"
#include "imgui.h"
#include "DatabaseManager.h" // Include your database manager


LoginWindow::LoginWindow() : showLoginWindow(true), loggedIn(false) {}

void LoginWindow::Initialize(GLFWwindow* window)
{
    this->window = window;
}
bool LoginWindow::ShowLoginWindow()
{
    return showLoginWindow;
}


void LoginWindow::Draw() {
  //  if (showLoginWindow)
  //  {

        ImGui::Begin("Login", &showLoginWindow);
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(1280, 720));
        ImGui::InputText("Username", usernameBuffer, IM_ARRAYSIZE(usernameBuffer));
        ImGui::InputText("Password", passwordBuffer, IM_ARRAYSIZE(passwordBuffer), ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login"))
        {


            DatabaseManager dbManager("C : \Stuff\sent_from_old_laptop\duty\OBD2_Diag_App\OBD2_App\Database.json");


            if (dbManager.VerifyCredentials(usernameBuffer, passwordBuffer))
            {
                loggedIn = true;
                userRole = dbManager.GetUserRole(usernameBuffer);
               // showLoginWindow = false; // Close the login window. You might want to set a flag to show the main window from the main loop
            }
            else
            {
                loginMessage = "Invalid credentials. Please try again.";


            }





       }




        ImGui::Text("%s", loginMessage.c_str());
        ImGui::End();


    

}


bool LoginWindow::IsLoggedIn() const
{
    return loggedIn;
}


std::string LoginWindow::GetUserRole() const
{
    return userRole;
}