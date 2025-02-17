#pragma once

#include <imgui.h>



class TechnicianWindow {
public:
    void Draw();


    void SetIsConnectedToServer(bool connected) { isConnectedToServer = connected; }

    void SetIsViewingData(bool viewing) { isViewingData = viewing; }

    void SetIsLogging(bool logging) { isLogging = logging; }



private:

    // Flags for UI state

    bool isConnectedToServer = false;

    bool isViewingData = false;
    bool isLogging = false;


};