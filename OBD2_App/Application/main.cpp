#include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Serial_port_connection.h>
#include <Communication_interfaces.h> 
#include <UserWindow.h>
#include <TechnicianWindow.h>
#include <LoginWindow.h>
#include <DatabaseManager.h> 
#include <Bluetooth_operations.h> 

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
int main(int argc, char** argv) 
{
    char exitKey;
    
    std::string elm327Command;
    int bluetooth_connection_status = 0;
    int serial_port_setup_status = 0;

   
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;


    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);



    GLFWwindow* window = glfwCreateWindow(1280, 720, "OBD2_App", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
   // DatabaseManager dbManager("C : \Stuff\sent_from_old_laptop\duty\OBD2_Diag_App\OBD2_App\Database\database.json"); //Provide path to your database

    LoginWindow loginWindow;
    UserWindow userWindow;  // Create UserWindow instance

    bool showUserWindow = false;  // Flag to control UserWindow visibility

 
    //std::cerr << "Error loading fonts. Exiting application." << std::endl;


    //// Shutdown ImGui and GLFW before exiting. This needs to be called before glfwDestroyWindow and glfwTerminate
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    //glfwDestroyWindow(window);
    //glfwTerminate();
    //exitKey = _getch();
    //return 1; // Return an error code to indicate failure

 // Main loop
    while (!glfwWindowShouldClose(window)) {
        // ... (ImGui new frame setup)

        if (!loginWindow.IsLoggedIn()) {
            loginWindow.Draw();


            if (loginWindow.IsLoggedIn() && loginWindow.GetUserRole() == "user") //Check here if login was successful and check the role
            {


                showUserWindow = true;

            }


        }
        else if (showUserWindow) {      //Show user window only after login is successful and correct credentials are provided for user
            userWindow.Draw();
        }

        // ... (ImGui rendering and buffer swap)
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "\nPress any key to exit" << std::endl;

    CloseBluetoothHandles();
    exitKey = _getch();

    return 0; 
}

