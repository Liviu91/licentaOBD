

//This program discovers and list all bluetooth devices in range and attempts to establish connections
//The program starts with looking at the status of bluetooth on computer(i.e. Local Radio) by calling IsBluetoothOn() 
//if it is Off then it attempts to open local radio of computer by calling func TurnOnBluetooth()
//if the function fails to open bluetooth, it prints to request user to enable bluetooth manually and restart program
//then when bluetooth radio of computer is enabled, the program attempts to find all the device within range and 
//finds the device named hc-05 by using the function FindBtDev(). This function if successful will return the data
//about bluetooth module to main function, if function fails then the program notify user that unable to find device and
//after some delay again looks for the device.
//finally when the device is found and data is return by FindBtDev() then status of connection and authentication of device
//is checked, if device is not authenticated (i.e false) then the pairDevice() function is called to attempt to pair with device.
//finally the BluetoothSetService function is used to set seiral serivce class and enable service of the device. this creates a
//virtual com.
//BUGS and ISSUES:
// the trunning on bluetooth radio function is not working at all. need to debug it. 

//==========Header============================
#include <Communication.h> 

HANDLE m_radio; //Windows handle to local radio, declared here as a global variable
HANDLE hBluetooth_port_operations;
HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle = 0;
int serial_port;
std::string userCommand;

int main(int argc, char** argv) //main code here.

{
	//======================variables===============================================
	char x;
	DWORD ret;
	const GUID serial = SerialPortServiceClass_UUID;
	bool retVal;
	int IsOn; //variable that tells whether function is working or not
	BLUETOOTH_DEVICE_INFO desired_device_info = { sizeof(BLUETOOTH_DEVICE_INFO),0, }; //a struct variable that will store info of desired device

//==========================================================================================			

	IsOn = IsBluetoothOn(); //start by checking wether bluetooth module is on or off

	if (IsOn == 0) { //i.e the device is not ON then//then ask user to turn on the bluetooth

		cout << "Enable bluetooth Manually and restart program" << endl;
		cout << "Press any key to exit" << endl;
		x = _getch();
		return 0;
	}
	else if (IsOn == -1) {
		cout << "Error Occured Closing Program" << endl;
		cout << "Press any key to exit" << endl;
		x = _getch();
		return 0;
	}

	//if IsOn==1 then local bluetooth is on 
	retVal = FindBtDev(&desired_device_info);
	if (retVal == false)
	{//device not found
		cout << "Error cannot find device" << endl;
		cout << "Searching again, Press any key to exit program" << endl;
		while (FindBtDev(&desired_device_info) == false) { //contiune looking until you find the device
			Sleep(1000);
			if (_getch()) { //if user press any key exit from the search
				CloseAllHandle();
				return 0;
			}
		}

	}

	if (!BluetoothIsConnectable(m_radio)) //check if local bluetooth radios accept incomming connections 
	{ //if not then attempt to turn this true
		cout << "Incomming connection was not ON, turning it On" << endl;
		if (BluetoothEnableIncomingConnections(m_radio, TRUE))
			cout << "Incomming connections enabled" << endl;
		else
			cout << "Error Unable to enable incoming connections" << endl;
	}
	else
		cout << "Incomming connection was ON!" << endl;

	if (desired_device_info.fConnected == FALSE && desired_device_info.fRemembered == TRUE) { //iff device is connected, then will we attempt to establish comport communication
		cout << "Device is out of range or switched Off." << endl;
	}



	if (desired_device_info.fRemembered == FALSE && desired_device_info.fConnected == FALSE) { //if the device is not remembered then attempt to create authentication and comport.
		cout << "Device Found Attempting to connect" << endl;

		if (desired_device_info.fAuthenticated == FALSE) { //if device is not authenticated then,
			BluetoothGetDeviceInfo(m_radio, &desired_device_info); //get updated device information
			if (!pairDevice(desired_device_info)) {//attempt to pair with the device.
				cout << "Authentication failed, Try manually" << endl;
				CloseAllHandle();
				return 0;
			}
		} 

		ret = BluetoothSetServiceState(m_radio, &desired_device_info, &serial, BLUETOOTH_SERVICE_ENABLE); //this will make the device as bluetooth com port hopefully
		if (ret != ERROR_SUCCESS && ret != E_INVALIDARG) {// if the operation is not successful and does not contain invalid argument
			if (ret == ERROR_INVALID_PARAMETER)
				cout << "Invalid Parameter" << endl;

			if (ret == ERROR_SERVICE_DOES_NOT_EXIST)
				cout << "Service not found" << endl;

			cout << "Press any key to exit" << endl;
			CloseAllHandle();
			x = _getch();
			return 0;
		}

		BluetoothGetDeviceInfo(m_radio, &desired_device_info); //get updated device infor

		BluetoothUpdateDeviceRecord(&desired_device_info);

		cout << "Name: " << desired_device_info.szName << endl;

		wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", desired_device_info.Address.rgBytes[5],

			desired_device_info.Address.rgBytes[4], desired_device_info.Address.rgBytes[3], desired_device_info.Address.rgBytes[2],

			desired_device_info.Address.rgBytes[1], desired_device_info.Address.rgBytes[0]);

		wprintf(L"  \tConnected: %s\r\n", desired_device_info.fConnected ? L"true" : L"false");
		wprintf(L"  \tAuthenticated: %s\r\n", desired_device_info.fAuthenticated ? L"true" : L"false");
		wprintf(L"  \tRemembered: %s\r\n", desired_device_info.fRemembered ? L"true" : L"false");


		cout << "Operation Successful check if comport created" << endl;
	}//end of if device is not remembered.

	//open created Bluetooth COM port and set the correct baudrates
	cout << "open created Bluetooth COM port and set the correct baudrates" << endl;
	
	if (Set_serial_port_connection(&hBluetooth_port_operations) == 0) {
		cout << "Baudrates succesfully set for the created com port" << endl;
	}
	else {
		cout << "Something is wrong! App is closing" << endl;
		return -1;
	}

	while (true) {
		std::cout << "Enter ELM327 command (or 'exit' to quit): ";
		std::getline(std::cin, userCommand); // Read entire line

		if (userCommand == "exit") {
			break; // Exit the loop if the user types "exit"
		}



		if (AT_Z_Command(&hBluetooth_port_operations, userCommand) == 0) { // Send command

			std::string elmResponse = readFromCOM3(&hBluetooth_port_operations); // Read response

			if (!elmResponse.empty()) {
				std::cout << "ELM327 response: " << elmResponse << std::endl;
			}
			else {
				std::cerr << "No response received from ELM327." << std::endl;
			}
		}
	}

	//Set_timeouts_for_SerialPortConnection(m_radio);
	//Send AT Z command to check communication and uC version
	/*cout << "Check communication by asking the ELMv327 it's version number" << endl;
	AT_Z_Command(&hBluetooth_port_operations);
	readFromCOM3(&hBluetooth_port_operations);*/

	cout << "\nPress any key to exit" << endl; 

	CloseAllHandle();
	x = _getch();

	return 0;

} //end of main