#include <Serial_port_connection.h>

int AT_Z_Command(HANDLE* hBluetooth_port, const std::string& command);

std::string readFromCOM3(HANDLE* hComPort);