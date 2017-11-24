//============================================================================
// Name        : SpiApp.cpp
// Author      : Yusuf Bülbül
// Version     :
// Copyright   : Your copyright notice
// Description : SpiApp in C++, Ansi-style
//============================================================================


#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <unistd.h>			//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "libSpi.h"
#include "max3107.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

using namespace std;

int main()
{

	int size;
	int register_address;
	unsigned char register_value;
	int selection;
	long spi_speed;
	unsigned char data[2];
	unsigned char readed_data[128];
	string baund_rate;
	string input_string;


	memset(&data[0], 145, sizeof(data));
//	cout << " Enter Spi Speed: " << endl << ">";
//	cin >> spi_speed;
	spi_speed = 26000000;

//	cout << " Enter BaundRate " << endl;
//	cin >> baund_rate;

	max3107 *max = new max3107(0, spi_speed);


	baund_rate = "MHZ8_B500000";
	max -> initMax((char *)baund_rate.c_str());
//	max -> writeUart(data, 2);

	while(true)
	{

		cout << endl;
		cout << "*Select 1, To Send Message" << endl;
		cout << "*Select 2, To Send File" << endl;
		cout << "*Select 3, To Listen the Port" << endl;
		cout << "*Select 4, To read uart" << endl;
		cout << "*Select 5, To write uart" << endl;
		cout << "*Select 6, To read register" << endl;
		cout << "*Select 7, To restart Max3107" << endl;
		cout << endl;
		cout << "> ";

		cin >> selection;


		if(selection == 1)
		{
			max->sendMessage();
			cout << endl << "> ";
		}
		else if(selection == 2)
		{
			max->sendFile();
			cout << endl << "> ";
		}
		else if(selection == 3)
		{
			max->listenPort();
			cout << endl << "> ";
		}
		else if(selection == 4)
		{
			cout << "Enter size of reading" << endl;
			cin >> size;
			max->readUart(readed_data, size, 0);
			printf("Message: ");
			for(int i=0; i<size; i++)
				printf("%c", readed_data[i]);
			printf("\n");

			cout << endl << "> ";
		}
		else if(selection == 5)
		{
			cout << "Enter message" << endl;
			cout << "> ";
			cin >> input_string;
			long transmitted_data_size;
			transmitted_data_size = TRANSMISSON_SIZE + 1;
			unsigned char transmitted_data[transmitted_data_size];

			max->writeUart((unsigned char *)input_string.c_str(), input_string.length());
			cout << endl << "> ";
		}
		else if(selection == 6)
		{
			cout << "Enter register address" << endl;
			cout << "> ";
			cin >> register_address;
			max->readRegister(register_address, &register_value);

			printf("VALUE: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(register_value));
			cout << endl << "> ";
		}
		else if(selection == 7)
		{
			cout << "Max3107 is Restarting..." << endl;
			max -> restartMax();
			cout << "> ";

		}
		else
		{
			printf("+Wrong Selection. Try Again!!!\n");
		}

		cout << "session is end" << endl;
	}


//		memset(&readed_data[0], 0, 26);
//		max -> readUart(readed_data, 26,);
//		printf("Data: ");
//		for(int i=0; i<26; i++)
//			printf("0x%02X-",readed_data[i]);
//		printf("\n");


		delete max;
}

