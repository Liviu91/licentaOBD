#include <Serial_port_connection.h>

//Open and det correct baudrate for the Bluetooth COM port
int Set_serial_port_connection(HANDLE* hBluetooth_port) {

	//LPCSTR serial_COM3 = L"\\\\.\\COM3";
	std::wstring serial_COM3 = L"\\\\.\\COM3";
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	*hBluetooth_port = CreateFile(
		serial_COM3.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (*hBluetooth_port == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			
			cout << "Serial port does not exist" << endl;
			return -1;
		}
		cout << "Other errors occured.Exit the program" << endl;
		return -1;
	}
		
	///-----------------------------Configure port------------------------------------------------------------

	if (!GetCommState(*hBluetooth_port, &serialParams)) {
		cout << "GetCommState: Error getting serial port state" << endl;
		return -1;
	}
	else {
		cout << "GetCommState: Serial port state returned succesfully" << endl;
	}
	/*serialParams.BaudRate = CBR_115200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;

	if (!SetCommState(hBluetooth_port, &serialParams)) {
		cout << "SetCommState: Error getting serial port state" << endl;
		return -1;
	}
	else {
		cout << "SetCommState: Serial port state returned succesfully" << endl;
	}*/
	
	return 0;
}

//Do we need it?
int Set_timeouts_for_SerialPortConnection(HANDLE hBluetooth_port) {

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = MAXDWORD;
	timeout.ReadTotalTimeoutConstant = 0;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 1;
	timeout.WriteTotalTimeoutMultiplier = 1;


	if (!SetCommTimeouts(hBluetooth_port, &timeout))
	{
		cout << "Error settimng serial port timeouts" << endl;
		return -1;
	}
	else {
		cout << "Serial port timeouts set correctly" << endl;
		return 0;
	}
}



