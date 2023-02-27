#include<windows.h>
#include<iostream>
using namespace std;

int main()
{
	HANDLE COM1;
	LPCTSTR Port1 = L"COM3";
	HANDLE COM2;
	LPCTSTR Port2 = L"COM4";
	COM1 = ::CreateFile(Port1, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(COM1 == INVALID_HANDLE_VALUE)
		cout << "COM-port_1 does not exist!\n";
	COM2 = ::CreateFile(Port2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DCB Params = { 0 };
	Params.DCBlength = sizeof(Params);
	if (!GetCommState(COM1, &Params))
		cout << "Getting state error.\n";
	Params.BaudRate = CBR_9600;
	Params.ByteSize = 8;
	Params.StopBits = ONESTOPBIT;
	Params.Parity = NOPARITY;
	if (!SetCommState(COM2, &Params))
		cout << "Error setting serial port state!\n";
	char Source = 'A';
	DWORD Size1 = sizeof(Source);
	DWORD RealCount;
	if(!WriteFile(COM1, &Source, Size1, &RealCount, NULL))
		cout << "Writting does not execute!\n";
	else
	{
		cout << Size1 << " Bytes in string; " << RealCount << " Bytes sended " << endl;
		DWORD Size2;
		char Result=' ';
		if (!ReadFile(COM2, &Result, 1, &Size2, 0))
			cout << "Reading does not execute!\n";
		else if (Size2 <= 0)
		{
			cout << "Something went wrong!\n";
		}
		else cout << Result << endl;
	}
	if(!CloseHandle(COM1))
		cout << "COM-port 1 does not close!\n";
	if (!CloseHandle(COM2))
		cout << "COM-port 2 does not close!\n";
	return 0;
}