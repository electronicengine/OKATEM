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




void SerialLib::closeUart()
{
//  ----- CLOSE THE UART -----
	close(gFileDescriptor);
}

void SerialLib::checkData(void (*uartCallBack)(unsigned char *RecievedData))
{

	int ret;
	unsigned char data_header[TRANSMISSION_HEADER_SIZE];
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];
	uint64_t size;

	if (gFileDescriptor != -1)
	{
		while(1)
		{

			ret = readHeader(&data_header[0], &data_size[0]);
			if(ret == SUCCESS)
			{
				if(strncmp((char*)data_header, "sendingf", TRANSMISSION_HEADER_SIZE) == 0)
				{
					size = stoll((char *)data_size);
					recieveFile(size);
				}
				else if(strncmp((char*)data_header, "sendingm", TRANSMISSION_HEADER_SIZE) == 0)
				{

					size = stoll((char *)data_size);
					recieveMessage(size);
				}
				else if(strncmp((char*)data_header, "sendingr", TRANSMISSION_HEADER_SIZE) == 0)
				{
					size = stoll((char *)data_size);
					recieveRandom(size);
				}
				else
				{
//					uartCallBack(&rx_buffer[0]);
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
	string input_string;
	uint64_t size;
	char *data_header = "sendingm";
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];

	cout << "+Enter Message: " << endl;
	cout << "> ";
    cin >> input_string;

    size = input_string.length();

	sprintf((char *)data_size, "%"PRId64"", size);

	writeHeader((unsigned char *)data_header, (unsigned char *)data_size);
	writeData((unsigned char *)input_string.c_str(), size);


}

int SerialLib::writeHeader(unsigned char* Header, unsigned char* Size)
{

	uint64_t unixtime;
	uint64_t unixtime0;
	uint64_t unixtime1;
	unsigned char unix_time0[TRANSMISSION_HEADER_SIZE];
	unsigned char unix_time1[TRANSMISSION_HEADER_SIZE];

	if (gFileDescriptor != -1)
	{

		int count = write(gFileDescriptor, Header, TRANSMISSION_HEADER_SIZE);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("+UART TX Header error\n");
			return FAIL;
		}

		cout << "+Data Size: " << Size << endl;
		count = write(gFileDescriptor, Size, TRANSMISSION_HEADER_SIZE);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("+UART TX DataSize error\n");
			return FAIL;
		}

		unixtime = time(0);
		unixtime0 = unixtime / 100000;
		unixtime1 = unixtime % 100000;

		sprintf((char *)&unix_time0[0], "%"PRId64"", unixtime0);
		cout << "+Sending start Time: " << unix_time0;

		sprintf((char *)&unix_time1[0], "%"PRId64"", unixtime1);
		cout << unix_time1 << endl;


		count = write(gFileDescriptor, unix_time0, TRANSMISSION_HEADER_SIZE);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("+UART TX UnixTime0 error\n");
			return FAIL;
		}
		count = write(gFileDescriptor, unix_time1, TRANSMISSION_HEADER_SIZE);		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("+UART TX UnixTime1 error\n");
			return FAIL;
		}
	}


	return SUCCESS;

}

int SerialLib::readHeader(unsigned char* Header, unsigned char* Size)
{
	static int data_count;
	int error_flag = 0;
	unsigned char rx_buffer[TRANSMISSION_HEADER_SIZE];
	unsigned char rx_package[TRANSMISSION_HEADER_SIZE*4];
	unsigned char unix_time0[TRANSMISSION_HEADER_SIZE];
	unsigned char unix_time1[TRANSMISSION_HEADER_SIZE];

	uint64_t unixtime;
	uint64_t unixtime0;
	uint64_t unixtime1;


	if (gFileDescriptor != -1)
	{
			// Read up to 255 characters from the port if they are there

		int rx_length = read(gFileDescriptor, (unsigned char *)&rx_buffer[0], TRANSMISSION_HEADER_SIZE);		//Filestream, buffer to store in, number of bytes to read (max)
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
		else if(rx_length == TRANSMISSION_HEADER_SIZE)
		{
			//Bytes received

			memcpy(&rx_package[data_count], &rx_buffer[0], TRANSMISSION_HEADER_SIZE);
			data_count = data_count + TRANSMISSION_HEADER_SIZE;

		}

		if(data_count == TRANSMISSION_HEADER_SIZE*4)
		{
			memcpy(Header, &rx_package[0], TRANSMISSION_HEADER_SIZE);
			memcpy(Size, &rx_package[1*TRANSMISSION_HEADER_SIZE], TRANSMISSION_HEADER_SIZE);

			cout << "+Data Size: " << Size << endl;

			memcpy(unix_time0, &rx_package[2*TRANSMISSION_HEADER_SIZE], TRANSMISSION_HEADER_SIZE);
			memcpy(unix_time1, &rx_package[3*TRANSMISSION_HEADER_SIZE], TRANSMISSION_HEADER_SIZE);

			cout << "+Recieving Start Time: " << unix_time0 << unix_time1 << endl;
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

int SerialLib::writeData(unsigned char* Data, long Size)
{
	int sequence;
	int remain_size;
	int count;
	uint64_t unixtime;
	unsigned char parted_data[TRANSMISSION_PACKAGE_SIZE];
    memset(parted_data, 0, TRANSMISSION_PACKAGE_SIZE);

	sequence = Size / TRANSMISSION_PACKAGE_SIZE;
	if((Size % TRANSMISSION_PACKAGE_SIZE) != 0)
		sequence++;

	for(int i=0; i<sequence; i++)
	{

		if(i == sequence - 1)
		{
			if(Size % TRANSMISSION_PACKAGE_SIZE == 0)
				remain_size = TRANSMISSION_PACKAGE_SIZE;
			else
				remain_size = Size % TRANSMISSION_PACKAGE_SIZE;

			memcpy(&parted_data[0], &Data[i*TRANSMISSION_PACKAGE_SIZE], remain_size);
			count = write(gFileDescriptor, &parted_data[0], TRANSMISSION_PACKAGE_SIZE);		//Filestream, bytes to write, number of bytes to write
			if (count < 0)
			{
				printf("+UART TX error\n");
				return FAIL;
			}

		}
		else
		{
			count = write(gFileDescriptor, &Data[i*TRANSMISSION_PACKAGE_SIZE], TRANSMISSION_PACKAGE_SIZE);		//Filestream, bytes to write, number of bytes to write
			if (count < 0)
			{
				printf("+UART TX error\n");
				return FAIL;
			}
		}

	}

	unixtime = time(NULL);
	cout << "Sending Finish Time: " << unixtime << endl;

	return SUCCESS;
}

int SerialLib::readData(unsigned char* Data, uint64_t Size)
{
	int sequence;
	int remain_size;
	int rx_buffer[TRANSMISSION_PACKAGE_SIZE];
	int ret;
	int data_count = 0;

	uint64_t unixtime;

	sequence = Size / TRANSMISSION_PACKAGE_SIZE;
	if(Size % TRANSMISSION_PACKAGE_SIZE != 0)
		sequence++;

	for(int i=0; i<sequence; i++)
	{

		if(i == sequence - 1)
		{


			if(Size % TRANSMISSION_PACKAGE_SIZE == 0)
				remain_size = TRANSMISSION_PACKAGE_SIZE;
			else
				remain_size = Size % TRANSMISSION_PACKAGE_SIZE;

			while(read(gFileDescriptor, (unsigned char *)&rx_buffer[0], TRANSMISSION_PACKAGE_SIZE) != TRANSMISSION_PACKAGE_SIZE);
			memcpy(&Data[data_count], &rx_buffer[0] ,remain_size);
			data_count = data_count + remain_size;

		}
		else
		{
			while(read(gFileDescriptor, (unsigned char *)&rx_buffer[0], TRANSMISSION_PACKAGE_SIZE) != TRANSMISSION_PACKAGE_SIZE);
			memcpy(&Data[i*TRANSMISSION_PACKAGE_SIZE], &rx_buffer[0] ,TRANSMISSION_PACKAGE_SIZE);
			data_count = data_count + TRANSMISSION_PACKAGE_SIZE;
		}

	}
	data_count = 0;

	unixtime = time(NULL);
	cout << "Recieving Finish Time: " << unixtime << endl;

//	do{ ret = read(gFileDescriptor, (unsigned char *)&rx_buffer[0], TRANSMISSION_PACKAGE_SIZE);
//		memcpy(&Data[data_count], &rx_buffer[0] ,ret);
//		data_count = data_count + ret;
//	}while(data_count < Size);
//	data_count = 0;


	return SUCCESS;
}

int SerialLib::recieveMessage(uint64_t DataSize)
{

	unsigned char *Message = new unsigned char[DataSize];

	readData(Message, DataSize);

	cout << Message << endl;
	return SUCCESS;
}


void SerialLib::sendRandom()
{
	int sequence;
	unsigned char *random_bytes;
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];
	char *data_header = "sendingr";
	long lSize;

	cout << "+Enter Data Sequence: " << endl;
	cout << "> ";
	cin >> sequence;

	lSize = sequence * TRANSMISSION_PACKAGE_SIZE;

	random_bytes = new unsigned char[lSize];

	cout << "+Generating Random Bytes..." << endl;

	for(int i=0; i<lSize; i++)
	{
		random_bytes[i] = rand()%255;
	}
	sprintf((char *)data_size, "%"PRId64"", lSize);

	cout << "+Sending Data..." << endl;
	writeHeader((unsigned char *)data_header, data_size);
	writeData(random_bytes, lSize);

	delete data_header;
}

int SerialLib::recieveRandom(uint64_t DataSize)
{
	int sequece;

	unsigned char *random_bytes = new unsigned char[DataSize];

	readData(random_bytes, DataSize);

	sequece = DataSize / TRANSMISSION_PACKAGE_SIZE;

	printf("Random Bytes: \n");
	for(int i = 0; i<sequece; i++)
	{
		for(int k = 0; k < TRANSMISSION_PACKAGE_SIZE; i++)
		{
			printf("%02X-",random_bytes);
		}
	}
	printf("\n");

	return SUCCESS;

}

int SerialLib::sendFile()
{


	FILE *pFile;

	uint64_t lSize;

	unsigned char *buffer;

	char *data_header = "sendingf";
	unsigned char data_size[lSize];

	size_t ret;

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

	// allocate memory to contain the whole file:
	buffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);
	if (buffer == NULL) {fputs ("+Memory error",stderr); fclose (pFile); free (buffer); return FAIL;}

	cout << "+Reading File..."<< endl;
	// copy the file into the buffer:
	ret = fread (buffer,1,lSize,pFile);
	if (ret != lSize) {fputs ("+Reading error",stderr); fclose (pFile); free (buffer); return FAIL;}

	/* the whole file is now loaded in the memory buffer. */
    cout << "+File Size: " << lSize << endl;
	cout << "+Sending File..." << endl;

	sprintf((char *)data_size, "%"PRId64"", lSize);

	writeHeader((unsigned char *)data_header, (unsigned char *)data_size);
	writeData((unsigned char *)buffer, lSize);

	// terminate
	fclose (pFile);
	free (buffer);

	delete data_header;
	return SUCCESS;

}

int SerialLib::recieveFile(uint64_t DataSize)
{

	FILE* fout;

	printf("+ALERT!!! File is recieving...\n\n");
	unsigned char *file_buffer = new unsigned char[DataSize];

	readData(file_buffer, DataSize);



	fout = fopen("recieved_file", "wb");
	if(fout != NULL)
	{
		fwrite(file_buffer, sizeof(unsigned char *), DataSize, fout);

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


