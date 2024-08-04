#include <Bluetooth_handling.h> 
#pragma warning(disable: 4995)
//HANDLE m_radio; //Windows handle to local radio, declared here as a global variable
//HBLUETOOTH_AUTHENTICATION_REGISTRATION hRegHandle = 0;

//return false if error occured and true if operation is successful
int IsBluetoothOn(void) {
	int ErrorCode;
	HANDLE r_radio;
	HBLUETOOTH_RADIO_FIND m_bt;
	BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) }; //a struct that needs to be passed
	int result;
	//do a search now to check
	m_bt = BluetoothFindFirstRadio(&m_bt_find_radio, &r_radio);
	if (m_bt != NULL) {
		cout << "Bluetooth Radio Found !" << endl;
		result = 1;
		if (CloseHandle(r_radio) == FALSE) {
			cout << "CloseHandle() failed with error code " << GetLastError() << endl;
			result = -1;
		} //close the handle


	}
	else {
		ErrorCode = GetLastError();
		result = -1;
		if (ErrorCode == ERROR_NO_MORE_ITEMS) {
			cout << "Bluetooth Module not enabled !" << endl;
			result = 0;
		}
		else

			cout << "Bluetooth Radio search failed with error code" << ErrorCode << endl;
	}



	return result;
}

bool FindBtDev(BLUETOOTH_DEVICE_INFO* desired_device_info) {
	//======================Handles============================================================

	HBLUETOOTH_RADIO_FIND m_bt;
	HBLUETOOTH_DEVICE_FIND m_bt_dev;
	//==========================================================================================
	//================VAriables=================================================================
	int m_radio_id;
	int m_device_id;
	DWORD mbtinfo_ret;
	bool FuncResult = false;

	//=================================================================================================
	//=======================Struct=====================================================================
	BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	BLUETOOTH_RADIO_INFO m_bt_info = { sizeof(BLUETOOTH_RADIO_INFO),0, };
	BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {
	sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
	 1,
	 0,
	 1,
	 1,
	 1,
	 15,
	 NULL };
	BLUETOOTH_DEVICE_INFO m_device_info = { sizeof(BLUETOOTH_DEVICE_INFO),0, };
	//================================================================================================
				// Iterate for available bluetooth radio devices in range
				// Starting from the local


	m_bt = BluetoothFindFirstRadio(&m_bt_find_radio, &m_radio);
	if (m_bt == NULL) {
		//cout<<"Bluetooth Radio Found !"<<endl;
		cout << "Bluetooth Radio failed with error code" << GetLastError() << endl;
		FuncResult = false;
	}


	m_radio_id = 0;


	// Then get the radio device info....      
	mbtinfo_ret = BluetoothGetRadioInfo(m_radio, &m_bt_info);

	if (mbtinfo_ret == ERROR_SUCCESS)

		cout << "Bluetooth Radio looksSSS fine!" << endl;

	else {

		cout << "BluetoothGetRadioInfo() failed wit error code " << mbtinfo_ret << endl;
		FuncResult = false;
	}

	m_search_params.hRadio = m_radio;

	ZeroMemory(&m_device_info, sizeof(BLUETOOTH_DEVICE_INFO));

	m_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);



	// Next for every radio, get the device

	m_bt_dev = BluetoothFindFirstDevice(&m_search_params, &m_device_info);


	if (m_bt_dev != NULL)

		cout << "BluetoothFindFirstDevice() is working!" << endl;

	else {

		cout << "Failed to find Devices with error code:  " << GetLastError() << endl;
		FuncResult = false;
	}



	m_radio_id++;

	m_device_id = 0;



	//Get the device info

	do

	{

		//-----this list all the bluetooth devices nearby, this can be commented out.
		wprintf(L"\n\tDevice %d:\r\n", m_device_id);

		wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);

		wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", m_device_info.Address.rgBytes[5],

			m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], m_device_info.Address.rgBytes[2],

			m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);

		wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);

		wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");

		wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");

		wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");

		m_device_id++;
		//------------------------------------------------------------------------

		if (wcscmp(m_device_info.szName, L"OBDII") == 0) //this search specifically for hc-05 bluetooth module.
		{
			*desired_device_info = m_device_info;
			FuncResult = true;
			break;
		}
		else {
			FuncResult = false;
		}


	} while (BluetoothFindNextDevice(m_bt_dev, &m_device_info));



	// NO more device, close the device handle

	if (BluetoothFindDeviceClose(m_bt_dev) == TRUE)

		cout << "BluetoothFindDeviceClose(m_bt_dev) is OK!" << endl;

	else {

		cout << "BluetoothFindDeviceClose(m_bt_dev) failed with error code " << GetLastError() << endl;
		FuncResult = false;

	}


	if (BluetoothFindRadioClose(m_bt) == TRUE)

		cout << "BluetoothFindRadioClose(m_bt) is OK!" << endl;

	else {

		cout << "BluetoothFindRadioClose(m_bt) failed with error code " << GetLastError() << endl;
		FuncResult = false;
	}

	return FuncResult;
}

bool pairDevice(BLUETOOTH_DEVICE_INFO device) {

	DWORD errorCode;
	bool result = false;
	//wchar_t passKey=L'1234\n';
	PWSTR* passKey = new PWSTR[1];
	passKey[0] = PWSTR(L"1234");// this is the default pass key/pin code for HC-05, can be changed to a custom value.
	errorCode = BluetoothAuthenticateDevice(NULL, m_radio, &device, *passKey, 4); //here 4 is the size of device passkey 

	switch (errorCode)
	{
	case(ERROR_SUCCESS):
		cout << "Device authenticated successfully" << endl;
		result = true;
		break;
	case(ERROR_CANCELLED):
		cout << "Device authenticated failed" << endl;
		result = false;
		break;
	case(ERROR_INVALID_PARAMETER):
		cout << "Invalid parameters" << endl;
		result = false;
		break;
	case(ERROR_NO_MORE_ITEMS):
		cout << "Device not available" << endl;
		result = false;
		break;
	}

	if (errorCode != ERROR_SUCCESS)
		cout << "Failure due to: " << GetLastError() << endl;

	return result;
}

void CloseAllHandle(void) {

	if (CloseHandle(m_radio) == FALSE) {
		cout << "CloseHandle() failed with error code " << GetLastError() << endl;
	}
	BluetoothUnregisterAuthentication(hRegHandle);

}
