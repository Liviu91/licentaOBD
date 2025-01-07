#include <Serial_port_connection.h>

int SendELM327Command(HANDLE* hBluetoothPort, const std::string& command);
std::string ReadELM327Response(HANDLE* hComPort);