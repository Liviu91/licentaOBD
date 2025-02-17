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
bool LoginWindow::LoadFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    defaultFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\times.ttf", 22.0f); //Change path if needed

    if (defaultFont == nullptr)
    {
        return false; //Return false on font load fail. The app should handle it.

    }

    return true; //Fonts are loaded correctly
}

void LoginWindow::Draw() {
    if (showLoginWindow)
    {

        // Center the login window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_FirstUseEver); // Center on first appearance


        ImGui::SetNextWindowSize(ImVec2(1000, 625)); // Set fixed window size


        ImGui::Begin("Login", &showLoginWindow, ImGuiWindowFlags_NoResize);

        ImGui::PushItemWidth(200); // Adjust width as needed

        // Calculate available vertical space (excluding frame height)
        float availableHeight = ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() * 2; // Only subtract frame height

        float elementsHeight = ImGui::GetTextLineHeightWithSpacing() * 2 + ImGui::GetStyle().ItemSpacing.y * 3 + 30; // Calculate total height of elements (2 input text + 3 spacings between + login button). Adjust values as needed

        float verticalOffset = (availableHeight - elementsHeight) * 0.5f; // Calculate vertical offset for centering

        ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() + verticalOffset); // Set initial vertical cursor position


        // Centered Username Label and Input
        auto usernameTextSize = ImGui::CalcTextSize("Username:");
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - (usernameTextSize.x + ImGui::GetStyle().ItemSpacing.x + 200)) * 0.5f);
        ImGui::Text("Username:");
        ImGui::SameLine();
        ImGui::InputText("##username", usernameBuffer, IM_ARRAYSIZE(usernameBuffer));

        // Centered Password Label and Input
        auto passwordTextSize = ImGui::CalcTextSize("Password:");
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - (passwordTextSize.x + ImGui::GetStyle().ItemSpacing.x + 200)) * 0.5f);
        ImGui::Text("Password:");
        ImGui::SameLine();
        ImGui::InputText("##password", passwordBuffer, IM_ARRAYSIZE(passwordBuffer), ImGuiInputTextFlags_Password);

        ImGui::PopItemWidth();

        // Centered Login Button (below input boxes)
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Login").x) * 0.5f - 100 / 2);

        if (ImGui::Button("Login"))
        {


            DatabaseManager dbManager("C : \Stuff\sent_from_old_laptop\duty\OBD2_Diag_App\OBD2_App\Database.json");


            if (dbManager.VerifyCredentials(usernameBuffer, passwordBuffer))
            {
                loggedIn = true;
                userRole = dbManager.GetUserRole(usernameBuffer);
                showLoginWindow = false; // Close the login window. You might want to set a flag to show the main window from the main loop
            }
            else
            {
                loginMessage = "Invalid credentials. Please try again.";


            }

        }



      }


    // Centered login message (below login button)
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(loginMessage.c_str()).x) * 0.5f);
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