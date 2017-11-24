/*
 * SerialLib.h
 *
 *  Created on: Oct 5, 2017
 *      Author: root
 */

#ifndef SERIALLIB_H_
#define SERIALLIB_H_

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <time.h>


#define SUCCESS 0
#define FAIL -1

#define TRANSMISSION_PACKAGE_SIZE 8
#define TRANSMISSION_SIZE 8


class SerialLib {
public:

	SerialLib();
	virtual ~SerialLib();

	unsigned char *generateRandom();
	int uartInit(char *BaundRate, char *Port);
	int readUart(unsigned char* RecievedData);
	int writeUart(unsigned char* TransmittedData);
	int parseDataAndSend(unsigned char *Data, long Size);

	void closeUart();
	void checkData(void (*uartCallBack)(unsigned char* RecievedData));
	void listenPort(void (*uartCallBack)(unsigned char* RecievedData));
	void sendMessage();
	void sendRandom(int Sequence);
	int sendFile();
	int recieveFile();

	int gFileDescriptor = -1;
	bool gListeningFlag = false;

private:



};

#endif /* SERIALLIB_H_ */
