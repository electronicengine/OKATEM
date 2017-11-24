//============================================================================
// Name        : UartApp-V2.cpp
// Author      : Yusuf Bülbül
// Version     :
// Copyright   : Your copyright notice
// Description : UartApp-V2 in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "SerialLib.h"
#include <time.h>
#include <string>

using namespace std;
long count = 0;
//
int sequence;

void uartCallBack(unsigned char *RecievedData);

void uartCallBack(unsigned char *RecievedData)
{
	count++;

	printf("+Sequence - Hex: %ld --- ", count);
	for(int i=0; i<TRANSMISSION_PACKAGE_SIZE; i++)
		printf("%02X-", RecievedData[i]);
	printf("\n");

	printf("+Sequence - Chr: %ld --- ", count);
		for(int i=0; i<TRANSMISSION_PACKAGE_SIZE; i++)
			printf("%c", RecievedData[i]);
	printf("\n");
	printf("\n");


}


int main()
{

	int ret;
	int selection;

	string baund_rate = "B115200";
	string port = "/dev/ttyAMA0";

	SerialLib *serial_bus = new SerialLib();


//	cout << "Welcome to Uart C++ App" << endl; // prints !!!Hello World!!!
//
//	cout<< "+Please Enter Baund Rate: \n(Options: B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000)" << endl;
//	cout << "> ";
//	cin >> baund_rate;
////
//	cout << "+Please Enter Port File Descriptor: \n(Options: /dev/ttyAMA0, /dev/ttyS0)" << endl;
//	cout << "> ";
//	cin >> port;

    ret = serial_bus->uartInit((char *)baund_rate.c_str(), (char *)port.c_str());
    if(ret != FAIL)
    {
    	printf("+Uart Port is opened Succesfully\n");

    	while(true)
    	{
    		cout << endl;
    		cout << "*Select 1, To Send 1024 Byte Random Bytes" << endl;
			cout << "*Select 2, To Send Message" << endl;
			cout << "*Select 3, To Send File" << endl;
			cout << "*Select 4, To Listen the Port" << endl;
			cout << endl;
			cout << "> ";

			cin >> selection;

			if(selection == 1)
			{
				serial_bus->sendRandom();
				cout << endl << "> ";
			}
			else if(selection == 2)
			{
				serial_bus->sendMessage();
				cout << endl << "> ";
			}
			else if(selection == 3)
			{
				serial_bus->sendFile();
				cout << endl << "> ";
			}
			else if(selection == 4)
			{
				serial_bus->listenPort(&uartCallBack);
				cout << endl << "> ";
			}
			else
			{
				printf("+Wrong Selection. Try Again!!!\n");
			}
    	}
    }
    else
    {
    	printf("+ERROR uartInit().\n");
    }

    delete serial_bus;

	return 0;
}
