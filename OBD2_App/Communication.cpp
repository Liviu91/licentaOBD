#include <Communication.h>

int AT_Z_Command(HANDLE hBluetooth_port) {

	DWORD dwBytesWritten, lasterror= 0x0;
    DWORD dwBytesRead = 0x4;
    char atz_command[] = "AT Z\r"; // I've also tried "ATZ\r", "ATI\n", "ATZ\n"
    char atz_reply[20]; 
    
    if (!WriteFile(hBluetooth_port, atz_command, dwBytesRead, &dwBytesWritten, NULL)) {
        cout << "Writing to serial port failed!" << endl;
            //return -1;
    }
    else {
        cout << "Writing on port succesfull!" << endl;
            return 0;
    } 
    lasterror = GetLastError(); 

}
