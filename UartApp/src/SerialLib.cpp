/*
 * SerialLib.cpp
 *
 *  Created on: Oct 5, 2017
 *      Author: root
 */

#include "SerialLib.h"


using namespace std;

SerialLib::SerialLib() {
	// TODO Auto-generated constructor stub

}

SerialLib::~SerialLib() {
	// TODO Auto-generated destructor stub
}

unsigned char* SerialLib::generateRandom()
{
	unsigned char* bytes = new unsigned char[TRANSMISSION_PACKAGE_SIZE];

	for(int i=0; i<TRANSMISSION_PACKAGE_SIZE; i++)
	{
		bytes[i]=rand() % 255;
	}

	return bytes;
}

int SerialLib::uartInit(char *BaundRate, char* Port)
{

	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively

	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	gFileDescriptor = open(Port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (gFileDescriptor == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("+Error - Unable to open UART.  Ensure it is not in use by another application\n");
		return FAIL;
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(gFileDescriptor, &options);
	if(strcmp(BaundRate,"B1200") == 0)
		options.c_cflag = B1200 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B2400") == 0)
		options.c_cflag = B2400 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B4800") == 0)
		options.c_cflag = B4800 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B9600") == 0)
		options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B19200") == 0)
		options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B38400") == 0)
		options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B57600") == 0)
		options.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B115200") == 0)
		options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B230400") == 0)
		options.c_cflag = B230400 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B460800") == 0)
		options.c_cflag = B460800 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B500000") == 0)
		options.c_cflag = B500000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B576000") == 0)
		options.c_cflag = B576000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B921600") == 0)
		options.c_cflag = B921600 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B1000000") == 0)
		options.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B1152000") == 0)
		options.c_cflag = B1152000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B1500000") == 0)
		options.c_cflag = B1500000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B2000000") == 0)
		options.c_cflag = B2000000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B2500000") == 0)
		options.c_cflag = B2500000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B3000000") == 0)
		options.c_cflag = B3000000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B3500000") == 0)
		options.c_cflag = B3500000 | CS8 | CLOCAL | CREAD;
	else if(strcmp(BaundRate,"B4000000") == 0)
		options.c_cflag = B4000000 | CS8 | CLOCAL | CREAD;
	else
	{
		printf("+Error - Unable to open UART.  Ensure it is not in use by another application\n");
		closeUart();
		return FAIL;
	}


	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(gFileDescriptor, TCIFLUSH);
	tcsetattr(gFileDescriptor, TCSANOW, &options);



	return SUCCESS;
}

int SerialLib::readUart(unsigned char *RecievedData)
{
	static int data_count;
	int error_flag = 0;
	unsigned char rx_buffer[TRANSMISSION_SIZE];
	unsigned char rx_package[TRANSMISSION_PACKAGE_SIZE];

	if (gFileDescriptor != -1)
	{
			// Read up to 255 characters from the port if they are there

		int rx_length = read(gFileDescriptor, (unsigned char *)&rx_buffer[0], TRANSMISSION_SIZE);		//Filestream, buffer to store in, number of bytes to read (max)
		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
			return FAIL;
		}
		else if (rx_length == 0)
		{
			//No data waiting
			return FAIL;
		}
		else
		{
			//Bytes received

			memcpy(&rx_package[data_count], &rx_buffer[0], TRANSMISSION_SIZE);
			data_count = data_count + TRANSMISSION_SIZE;

		}

		if(data_count == TRANSMISSION_PACKAGE_SIZE)
		{
			memcpy(RecievedData, &rx_package[0], TRANSMISSION_PACKAGE_SIZE);
			data_count = 0;
			return SUCCESS;
		}
		else
		{
			return FAIL;
		}

	}
	else
	{
		return FAIL;
	}
}

int SerialLib::writeUart(unsigned char* TransmittedData)
{
	//----- TX BYTES -----

	if (gFileDescriptor != -1)
	{
		for(int i=0; i<(TRANSMISSION_PACKAGE_SIZE/TRANSMISSION_SIZE); i++)
		{
			int count = write(gFileDescriptor, &TransmittedData[i*TRANSMISSION_SIZE], TRANSMISSION_SIZE);		//Filestream, bytes to write, number of bytes to write
			if (count < 0)
			{
				printf("+UART TX error\n");
				return FAIL;
			}

		}
		usleep(1);
	}


	return SUCCESS;
}

void SerialLib::closeUart()
{
//  ----- CLOSE THE UART -----
	close(gFileDescriptor);
}

void SerialLib::checkData(void (*uartCallBack)(unsigned char *RecievedData))
{

	int ret;

	if (gFileDescriptor != -1)
	{
		while(1)
		{
			unsigned char rx_buffer[TRANSMISSION_PACKAGE_SIZE];
			ret = readUart(&rx_buffer[0]);
			if(ret == SUCCESS)
			{
				if(strncmp((char*)rx_buffer, "sendingf", TRANSMISSION_PACKAGE_SIZE) == 0)
				{
					recieveFile();
				}
				else
				{
					uartCallBack(&rx_buffer[0]);
				}
			}

		}
	}
	else
	{
		printf("+ERROR! Port is not Open\n");
	}

}

void SerialLib::listenPort(void (*uartCallBack)(unsigned char *RecievedData))
{
	if(gListeningFlag == false)
	{
		printf("+Listening the Uart Port...\n");

		thread listen_thread(&SerialLib::checkData, this, uartCallBack);
		listen_thread.detach();
		gListeningFlag = true;
	}
	else
	{
		printf("+The Port is already listening.\n");
	}

}

void SerialLib::sendMessage()
{
	string bytes;
	unsigned char parted_bytes[TRANSMISSION_PACKAGE_SIZE];
	int size;
	int sequence;
	int remain_size;

	cout << "+You can send 8 Bytes in the one time" << endl;
	cout << "+Enter Message with no space: " << endl;
	cout << "> " << endl;
	cin >> bytes;

	size = bytes.length();
	sequence = size / TRANSMISSION_PACKAGE_SIZE;
	if((size % TRANSMISSION_PACKAGE_SIZE) != 0)
		sequence++;

	for(int i=0; i<sequence; i++)
	{

		if(i == sequence - 1)
		{
			if(size % TRANSMISSION_PACKAGE_SIZE == 0)
				remain_size = 8;
			else
				remain_size = size % TRANSMISSION_PACKAGE_SIZE;

			memcpy(&parted_bytes[0], &bytes[i*TRANSMISSION_PACKAGE_SIZE],remain_size);
			writeUart(&parted_bytes[0]);
		}
		else
		{
			memcpy(&parted_bytes[0], &bytes[i*TRANSMISSION_PACKAGE_SIZE], TRANSMISSION_PACKAGE_SIZE);
			writeUart(&parted_bytes[0]);
		}

	}
}

int SerialLib::parseDataAndSend(unsigned char* Data, long Size)
{
	unsigned char parted_bytes[TRANSMISSION_PACKAGE_SIZE];

	int sequence;
	int remain_size;


	sequence = Size / TRANSMISSION_PACKAGE_SIZE;
	if((Size % TRANSMISSION_PACKAGE_SIZE) != 0)
		sequence++;

	for(int i=0; i<sequence; i++)
	{

		if(i == sequence - 1)
		{
			if(Size % TRANSMISSION_PACKAGE_SIZE == 0)
				remain_size = 8;
			else
				remain_size = Size % TRANSMISSION_PACKAGE_SIZE;

			memcpy(&parted_bytes[0], &Data[i*TRANSMISSION_PACKAGE_SIZE],remain_size);
			writeUart(&parted_bytes[0]);
		}
		else
		{
			memcpy(&parted_bytes[0], &Data[i*TRANSMISSION_PACKAGE_SIZE], TRANSMISSION_PACKAGE_SIZE);
			writeUart(&parted_bytes[0]);
		}

	}
	return SUCCESS;
}

void SerialLib::sendRandom(int Sequence)
{
	unsigned char random_bytes[8];

	random_bytes[0] = 0x32;
	random_bytes[1] = 0x32;
	random_bytes[2] = 0x32;
	random_bytes[3] = 0x32;
	random_bytes[4] = 0x32;
	random_bytes[5] = 0x32;
	random_bytes[6] = 0x32;
	random_bytes[7] = 0x32;



	long time0 = time(NULL);
	printf("%ld\n", time0);
	for(int i=0; i<Sequence; i++)
	{
//		cout << "Transition Sequence: " << Sequence << endl;
//		random_bytes = generateRandom();

//		printf("Sent Random Bytes: ");
//		for(int i=0; i<TRANSMISSION_SIZE; i++)
//			printf("%02X-", random_bytes[i]);
//		printf("\n");

		writeUart(&random_bytes[0]);

//		delete random_bytes;
	}
}

int SerialLib::sendFile()
{


	FILE *pFile;

	uint64_t lSize;
	uint64_t start_time;
	uint64_t finish_time;
	uint64_t time0;
	uint64_t time1;
	uint64_t sequence;

	double speed;

	unsigned char *buffer;
	unsigned char file_size[8];
	unsigned char unix_time0[8];
	unsigned char unix_time1[8];
//
	size_t result;

	string file_name;

	cout << "+Please enter File Locatiton" << endl;
	cout << "> ";
	cin >> file_name;

	pFile = fopen (file_name.c_str() , "rb" );
	if (pFile==NULL) {fputs ("+File error",stderr); return FAIL;}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	sequence = lSize / TRANSMISSION_PACKAGE_SIZE;
	if(lSize % TRANSMISSION_PACKAGE_SIZE != 0)
		sequence++;

	// allocate memory to contain the whole file:
	buffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);
	if (buffer == NULL) {fputs ("+Memory error",stderr); fclose (pFile); free (buffer); return FAIL;}

	cout << "+Reading File..."<< endl;
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("+Reading error",stderr); fclose (pFile); free (buffer); return FAIL;}

	/* the whole file is now loaded in the memory buffer. */

	cout << "+Sending File..." << endl;
	start_time = time(NULL);


	time0 = start_time / 100000;
	time1 = (uint64_t) start_time % (uint64_t) 100000;



    sprintf((char *) &unix_time0[0], "%"PRId64"",time0);



    sprintf((char *) &unix_time1[0], "%"PRId64"",time1);



    sprintf((char *) &file_size[0], "%"PRId64"",lSize);

    cout << "+File Size: " << file_size << endl;

	writeUart((unsigned char *)"sendingf"); // file_descriptor is sending

	writeUart(&file_size[0]);
	writeUart(&unix_time0[0]);
	writeUart(&unix_time1[0]);


	parseDataAndSend(buffer, lSize);
	finish_time = time(NULL);
	speed = (double)((uint64_t)lSize / (double)((uint64_t)finish_time - (uint64_t)start_time)) * 8;
	speed = speed / 1000000 ;

	cout << "+Transmission is finished!" << endl;
	cout << "+Start Time: " << start_time << endl;
	cout << "+Finish Time: " << finish_time << endl;
	printf("+Transmission Speed: %4.2f MBPS\n",speed);

	// terminate
	fclose (pFile);
	free (buffer);

	return SUCCESS;

}

int SerialLib::recieveFile()
{

	uint64_t file_size[8];
	uint64_t finish_time;
	uint64_t start_time;
	uint64_t time0;
	uint64_t time1;
	uint64_t lSize;

	int remain_size;
	int ret;
	int sequence;
	double speed;

	FILE* fout;

	unsigned char rx_buffer[TRANSMISSION_PACKAGE_SIZE];
	unsigned char *file_buffer;

	printf("+ALERT!!! File is recieving...\n\n");

	while(readUart(&rx_buffer[0]) != SUCCESS);
	lSize = stoll((char *)&rx_buffer[0]);
	cout << "+Recieving File Size: " << lSize << endl;

	while(readUart(&rx_buffer[0]) != SUCCESS);
	time0 = stoll((char *)&rx_buffer[0]);


	while(readUart(&rx_buffer[0]) != SUCCESS);
	time1 = stoll((char *)&rx_buffer[0]);

	start_time = (time0 * 100000) + time1;


	file_buffer = new unsigned char[lSize];

	sequence = lSize / TRANSMISSION_PACKAGE_SIZE;
	if((lSize % TRANSMISSION_PACKAGE_SIZE) != 0)
		sequence++;

	for(int i=0; i<sequence; i++)
	{

		while(readUart(&rx_buffer[0]) != SUCCESS);

		if(i == sequence - 1)
		{
			if(lSize % TRANSMISSION_PACKAGE_SIZE == 0)
				remain_size = 8;
			else
				remain_size = lSize % TRANSMISSION_PACKAGE_SIZE;

			memcpy(&file_buffer[i*TRANSMISSION_PACKAGE_SIZE], &rx_buffer[0] ,remain_size);
		}
		else
		{
			memcpy(&file_buffer[i*TRANSMISSION_PACKAGE_SIZE], &rx_buffer[0] ,TRANSMISSION_PACKAGE_SIZE);
		}

	}

	finish_time = time(NULL);
	speed = (double)((uint64_t)lSize / (double)((uint64_t)finish_time - (uint64_t)start_time)) * 8;
	speed = speed / 1000000 ;

	cout << "+Transmission is finished!" << endl;
	cout << "+Start Time: " << start_time << endl;
	cout << "+Finish Time: " << finish_time << endl;
	printf("+Transmission Speed: %4.2f MBPS\n",speed);

	fout = fopen("recieved_file", "wb");
	if(fout != NULL)
	{
		fwrite(file_buffer, sizeof(unsigned char *), lSize, fout);

		fclose(fout);
	}
	else
	{
		printf("+ERROR! The File can not created\n");
		fclose(fout);
		delete file_buffer;
		return FAIL;
	}

	delete file_buffer;
	return SUCCESS;

}
