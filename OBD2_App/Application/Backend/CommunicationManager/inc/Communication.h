//#include <algorithm> // required for std::remove
//#include <cctype>    // required for std::iscntrl

int SendELM327Command(HANDLE* hBluetoothPort, const std::string& command);
std::string ReadELM327Response(HANDLE* hComPort);
int InitializeELM327(HANDLE* hBluetoothPort);