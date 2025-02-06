#include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Bluetooth_operations.h> 
#include <Serial_port_connection.h>
#include <Communication.h> 
#include <MainWindow.hpp>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
int main(int argc, char** argv) 
{
    char exitKey;
    HANDLE hBluetoothSerialPort;
    std::string elm327Command;
    int bluetooth_connection_status = 0;
    int serial_port_setup_status = 0;

    //if (!Connect_With_ELM327_via_Bluetooth()) {
    //    std::cout << "Error setting up bluetooth connection with ELM327 device" << std::endl;
    //    std::cout << "Try again..." << std::endl;
    //}
    //else // Set up the serial port connection.
    //    if (!SetupSerialPort(&hBluetoothSerialPort)) {
    //        // Serial port setup was successful.
    //        std::cout << "Try again..." << std::endl;
    //    }
    //    else {
    //        /*Initialize ELM327:
    //         * version, vin, ecu id, at e0? maybe more info to showcase at the beggining of the mainWindow*/
    //        if (!InitializeELM327(&hBluetoothSerialPort)) {
    //            std::cout << "Init succesfull!" << std::endl;
    //        }
    //        else {
    //            std::cerr << "Init failed!" << std::endl;
    //            return -1;
    //        }
    //    }

   
    //// Main command loop.
    //while (true) {
    //    std::cout << "Enter ELM327 command (or 'exit' to quit): ";
    //    std::getline(std::cin, elm327Command); // Read input command.

    //    if (elm327Command == "exit") {
    //        break; // Exit the loop when the user enters "EXIT".
    //    }

    //    if (SendELM327Command(&hBluetoothSerialPort, elm327Command) == 0) { // Try to send a command.
    //        std::string elm327Response = ReadELM327Response(&hBluetoothSerialPort); // Read response from ELM327.

    //        if (!elm327Response.empty()) {
    //            std::cout << elm327Response << std::endl; 
    //        }
    //        else {
    //            std::cerr << "No response from ELM327." << std::endl;
    //        }
    //    }
    //}

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
        return -1;
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



    MainWindow mainWindow;  // Create MainWindow instance
    if (!mainWindow.LoadFonts()) // Load fonts and check for errors
    {
        std::cerr << "Error loading fonts. Exiting application." << std::endl;


        // Shutdown ImGui and GLFW before exiting. This needs to be called before glfwDestroyWindow and glfwTerminate
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();



        glfwDestroyWindow(window);
        glfwTerminate();
        return 1; // Return an error code to indicate failure

    }
    mainWindow.Initialize(window); //Initialize main window


    while (!glfwWindowShouldClose(window))
    {


        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        mainWindow.Draw();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    cout << "\nPress any key to exit" << endl; 

    CloseBluetoothHandles();
    exitKey = _getch();

    return 0; 
}

