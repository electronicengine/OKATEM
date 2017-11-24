/*
 * max3107.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: root
*/

#include "max3107.h"

using namespace std;

max3107::max3107(int SpiDevice, long SpiSpeed)
{


	int ret;
	gSpi = new libSpi();

	ret = gSpi->SpiOpenPort(SpiDevice, SpiSpeed);
	gSpiDevice = SpiDevice;

	if(!(ret < 0))
		printf("spidev0.%d is opened succesfully\n", SpiDevice);
	else
		printf("spidev0.%d can not opened\n");
}


max3107::~max3107()
{
	// TODO Auto-generated destructor stub
	gSpi->SpiClosePort(gSpiDevice);
	delete gSpi;
}


int max3107::initMax(char *BaundRate)
{
	int register_address;
	int register_value;
	uint16_t baund_rate;
	unsigned char value;

	wiringPiSetup ();
	system("gpio edge 1 falling"); // falling edge interrupt for IRQ_PIN

	pinMode (POWER_PIN, OUTPUT);
	pinMode (IRQ_PIN, INPUT);
	pinMode (BUSY_PIN, OUTPUT);
	pinMode (BUSY_FLAG_PIN, INPUT);

	digitalWrite(POWER_PIN, HIGH);
	sleep(1);
	digitalWrite(POWER_PIN, LOW);

	digitalWrite(BUSY_PIN, LOW);


	/* 1. Configure baud rate*/
	if(strcmp(BaundRate, "MHZ20_B9600") == 0)
		baund_rate = MHZ20_B9600;
	else if(strcmp(BaundRate, "MHZ20_B38400") == 0)
		baund_rate = MHZ20_B38400;
	else if(strcmp(BaundRate, "MHZ20_B57600") == 0)
		baund_rate = MHZ20_B57600;
	else if(strcmp(BaundRate, "MHZ20_B115200") == 0)
		baund_rate = MHZ20_B115200;
	else if(strcmp(BaundRate, "MHZ20_B230400") == 0)
		baund_rate = MHZ20_B230400;
	else if(strcmp(BaundRate, "MHZ20_B460800") == 0)
		baund_rate = MHZ20_B460800;
	else if(strcmp(BaundRate, "MHZ20_B921600") == 0)
		baund_rate = MHZ20_B921600;
	else if(strcmp(BaundRate, "MHZ20_B1000000") == 0)
		baund_rate = MHZ20_B1000000;
	else if(strcmp(BaundRate, "MHZ20_B2000000") == 0)
		baund_rate = MHZ20_B2000000;
	else if(strcmp(BaundRate, "MHZ8_B9600") == 0)
		baund_rate = MHZ8_B9600;
	else if(strcmp(BaundRate, "MHZ8_B38400") == 0)
		baund_rate = MHZ8_B38400;
	else if(strcmp(BaundRate, "MHZ8_B57600") == 0)
		baund_rate = MHZ8_B57600;
	else if(strcmp(BaundRate, "MHZ8_B115200") == 0)
		baund_rate = MHZ8_B115200;
	else if(strcmp(BaundRate, "MHZ8_B230400") == 0)
		baund_rate = MHZ8_B230400;
	else if(strcmp(BaundRate, "MHZ8_B265000") == 0)
		baund_rate = MHZ8_B265000;
	else if(strcmp(BaundRate, "MHZ8_B276000") == 0)
		baund_rate = MHZ8_B276000;
	else if(strcmp(BaundRate, "MHZ8_B285000") == 0)
		baund_rate = MHZ8_B285000;
	else if(strcmp(BaundRate, "MHZ8_B300000") == 0)
		baund_rate = MHZ8_B300000;
	else if(strcmp(BaundRate, "MHZ8_B308000") == 0)
		baund_rate = MHZ8_B308000;
	else if(strcmp(BaundRate, "MHZ8_B320102") == 0)
		baund_rate = MHZ8_B320102;
	else if(strcmp(BaundRate, "MHZ8_B333000") == 0)
		baund_rate = MHZ8_B333000;
	else if(strcmp(BaundRate, "MHZ8_B348000") == 0)
		baund_rate = MHZ8_B348000;
	else if(strcmp(BaundRate, "MHZ8_B364000") == 0)
		baund_rate = MHZ8_B364000;
	else if(strcmp(BaundRate, "MHZ8_B381000") == 0)
		baund_rate = MHZ8_B381000;
	else if(strcmp(BaundRate, "MHZ8_B400000") == 0)
		baund_rate = MHZ8_B400000;
	else if(strcmp(BaundRate, "MHZ8_B421000") == 0)
		baund_rate = MHZ8_B421000;
	else if(strcmp(BaundRate, "MHZ8_B444000") == 0)
		baund_rate = MHZ8_B444000;
	else if(strcmp(BaundRate, "MHZ8_B460800") == 0)
		baund_rate = MHZ8_B460800;
	else if(strcmp(BaundRate, "MHZ8_B500000") == 0)
		baund_rate = MHZ8_B500000;
	else if(strcmp(BaundRate, "MHZ8_B921600") == 0)
		baund_rate = MHZ8_B921600;
	else if(strcmp(BaundRate, "MHZ8_B1000000") == 0)
		baund_rate = MHZ8_B1000000;
	else if(strcmp(BaundRate, "MHZ8_B2000000") == 0)
		baund_rate = MHZ8_B2000000;
	else
		baund_rate = MHZ8_B9600; //Default baund_rate

	register_address = DIVMSB_REGISTER_ADDRESS;
	register_value = 0;
	register_value = (baund_rate >> 16) & SPI_TX_DATA_MASK;
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(DIVMSB_REGISTER_ADDRESS);

	register_address = DIVLSB_REGISTER_ADDRESS;
	register_value = 0;
	register_value = (baund_rate >> 8) & SPI_TX_DATA_MASK ;
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(DIVLSB_REGISTER_ADDRESS);

	register_address = BRGCONFIG_REGISTER_ADDRESS;
	register_value = baund_rate & SPI_TX_DATA_MASK;
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(BRGCONFIG_REGISTER_ADDRESS);

	/* 2. Configure LCR register, 8N1 mode by default */
	register_address = LCR_REGISTER_ADDRESS;
	register_value = 0;
	register_value = LCR_WORD_LEN_8;
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(LCR_REGISTER_ADDRESS);

	/* 3. Configure MODE 1 register */
	register_address = MODE1_REGISTER_ADDRESS;
	register_value = 0;
	/* Enable IRQ pin */
	register_value |= MODE1_IRQSEL_BIT;
	/* Disable TX */
//	register_value = MODE1_TXDIS_BIT;
	/* RX is enabled */
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(MODE1_REGISTER_ADDRESS);

	/* 4. Configure MODE 2 register */
	register_address = MODE2_REGISTER_ADDRESS;
	register_value = 0;
	/* Enable loopback */
//	register_value |= MODE2_LOOPBACK_BIT;
	/* Reset FIFOs */
	register_value |=MODE2_FIFORST_BIT;
	writeRegister(register_address, register_value);
	register_value = 0;
//	register_value |=MODE2_ECHOSUPR_BIT;
	writeRegister(register_address, register_value); //Reset bit must be set 0 for resume normal oeration

//	usleep(1000);
//	readRegister(MODE2_REGISTER_ADDRESS);

	/* 5. Configure FIFO trigger level register */
	register_address = FIFOTRGLVL_REGISTER_ADDRESS;
	register_value = 0;
	/* RX FIFO trigger for 16 words, TX FIFO trigger not used */
	/* RX FIFO trigger for 16 words, TX FIFO trigger for 16 words */
	register_value |= FIFOTRIGLVL_RX(88) | FIFOTRIGLVL_TX(96);
	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(FIFOTRGLVL_REGISTER_ADDRESS);

	/* 6. Configure flow control levels */
//	register_address = FLOWLVL_REGISTER_ADDRESS;
	/* Flow control halt level 96, resume level 48 */
	register_value = 0;
//	register_value |= (FLOWLVL_RES(48) | FLOWLVL_HALT(96));
//	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(FLOWLVL_REGISTER_ADDRESS);

	/* 7. Configure flow control */
//	register_address = FLOWCTRL_REGISTER_ADDRESS;
//	register_value = 0;
	/* Enable auto CTS and auto RTS flow control */
//	register_value |= FLOWCTRL_AUTOCTS_BIT;
//	register_value |= FLOWCTRL_AUTORTS_BIT;
//	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(FLOWCTRL_REGISTER_ADDRESS);

	/* 8. Configure RX timeout register */
//	register_address = RXTIMEOUT_REGISTER_ADDRESS;
//	register_value = 0;
	/* Timeout after 48 character intervals */
//	register_value = 0x30;
//	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(RXTIMEOUT_REGISTER_ADDRESS);

	/* 9. Configure LSR interrupt enable register */
//	register_address = LSRINTEN_REGISTER_ADDRESS;
//	register_value = 0;
//	register_value |= LSR_RXTO_BIT;
//	writeRegister(register_address, register_value);

//	usleep(1000);
//	readRegister(LSRINTEN_REGISTER_ADDRESS);

	/* 10. Clear IRQ status register by reading it */
	readRegister(ISR_REGISTER_ADDRESS, &value);

	/* 11. Configure interrupt enable register */
	/* Enable LSR interrupt */
	register_address = IRQEN_REGISTER_ADDRESS;
	register_value = 0;
	/* Enable LSR interrupt */
//	register_value |= IRQ_LSR_BIT;
	/* Enable RX FIFO interrupt */
	register_value |= IRQ_RXFIFO_BIT;
//	register_value |= IRQ_RXEMPTY_BIT;
	/* Enable TX FIFO interrupt */
	register_value |= IRQ_TXEMPTY_BIT;
//	register_value |= IRQ_CTS_BIT;
	writeRegister(register_address, register_value);

	/* 12. Configure PLL Register for 20MHZ */
	register_address = PLLCONFIG_REGISTER_ADDRESS;
	register_value = 0;
	register_value = 1;
//	register_value |= PLLCFG_FACTOR1;
	writeRegister(register_address, register_value);

	/* 12. Enable Internal Clock 614.4kHz */
	register_address = CLKSOURCE_REGISTER_ADDRESS;
	register_value = 0;
//	register_value |= CLKSRC_CRYST_BIT | CLKSRC_EXTCLK_BIT | CLKSRC_PLLBYP_BIT | CLKSRC_CLK2RTS_BIT;
	register_value |= CLKSRC_EXTCLK_BIT | CLKSRC_PLLBYP_BIT | CLKSRC_CLK2RTS_BIT;
//	register_value |= CLKSRC_CRYST_BIT | CLKSRC_EXTCLK_BIT | CLKSRC_PLLBYP_BIT;
//	register_value |= CLKSRC_INTOSC_BIT | CLKSRC_PLLBYP_BIT | CLKSRC_CLK2RTS_BIT;
	writeRegister(register_address, register_value);

	/* 13. Enable STSInt */
	register_address = STSINT_REGISTER_ADDRESS;
	register_value = 0;
	register_value |= STS_CLKREADY_BIT;
	writeRegister(register_address, register_value);

	/* 14. Enable STSInt */
	register_address = STSINTEN_REGISTER_ADDRESS;
	register_value = 0;
	register_value |= STS_CLKREADY_BIT;
	writeRegister(register_address, register_value);

	readRegister(UART_RECIEVER_HOLD_REGISTER, &value);


}

void max3107::resetFifo()
{
	int ret;

	long transmitted_data_size;
	int register_address;
	int register_value;
	uint16_t baund_rate;
	unsigned char value;
	unsigned char transmitted_data[transmitted_data_size];

//
//	transmitted_data[0] = UART_TRANSMIT_HOLD_REGISTER;  // Write Command
//	transmitted_data[0] |= WRITE_BIT;
//	memset(&transmitted_data[1], 0x20, TRANSMISSON_SIZE);
//	gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], transmitted_data_size);
//
//	readRegister(ISR_REGISTER_ADDRESS, &transmitted_data[2]);

//	do{
//		ret = digitalRead(IRQ_PIN);
//	}while(ret);
//	readRegister(ISR_REGISTER_ADDRESS, &value); // reset interrupt
//
//	do{
//		ret = digitalRead(IRQ_PIN);
//	}while(ret);
//	readRegister(ISR_REGISTER_ADDRESS, &value); // reset interrupt

	register_address = MODE2_REGISTER_ADDRESS;
	register_value = 0;
	register_value |=MODE2_FIFORST_BIT;
	writeRegister(register_address, register_value);
	writeRegister(register_address, 0); //Reset bit must be set 0 for resume normal oeration

}

int max3107::writeRegister(int RegisterAddress, int Value)
{
	int ret;
	unsigned char transmitted_data[2];

	transmitted_data[0] = RegisterAddress;
	transmitted_data[0] |= WRITE_BIT;
	transmitted_data[1] = Value;

	ret = gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], sizeof(transmitted_data));
	if(ret < 0)
		printf("SpiWrite Error!");

	return SUCCESS;
}


int max3107::readRegister(int RegisterAddress, unsigned char *RegisterValue)
{
	int ret;
	unsigned char transmitted_data[2];

	transmitted_data[0] = RegisterAddress;
	transmitted_data[1] = 0;

	ret = gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], 2);
	if(ret < 0)
		printf("SpiWrite Error!");

	memcpy(RegisterValue, &transmitted_data[1], 1);

	return SUCCESS;
}

int max3107::writeUart(unsigned char* Data, long Size)
{
	unsigned long long count = 0;
	long long count1 = 0;
	int ret;
	long sequence;
	int remain_size;
	int transmitted_data_size = TRANSMISSON_SIZE + 8;
	unsigned char transmitted_data[transmitted_data_size];
	unsigned char register_value;
	int register_address;

	sequence = Size / TRANSMISSON_SIZE;
	if(Size % TRANSMISSON_SIZE != 0)
		sequence++;

	for(long i=0; i<sequence; i++)
	{
		transmitted_data[0] = UART_TRANSMIT_HOLD_REGISTER;  // Write Command
		transmitted_data[0] |= WRITE_BIT;
		memset(&transmitted_data[1], 0x20, transmitted_data_size - 1);

		if(i == sequence - 1)
		{
			remain_size = Size % TRANSMISSON_SIZE;

			memcpy(&transmitted_data[1], &Data[i*TRANSMISSON_SIZE], remain_size);

			printf("Sequence%d: ", i);
			for(int k=1; k<TRANSMISSON_SIZE; k++)
				printf("%02X-", transmitted_data[k]);
			printf("\n\n");

			ret = gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], transmitted_data_size);
			if(ret < 0)
				printf("SpiWrite Error!");

		}
		else
		{
			memcpy(&transmitted_data[1], &Data[i*TRANSMISSON_SIZE], TRANSMISSON_SIZE);

			printf("Sequence%d: ", i);
			for(int k=1; k<TRANSMISSON_SIZE; k++)
				printf("%02X-", transmitted_data[k]);
			printf("\n\n");

			ret = gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], transmitted_data_size);
			if(ret < 0)
				printf("SpiWrite Error!");
		}


		while(digitalRead(IRQ_PIN))
			count++;

		if(count == 0)
		{
			readRegister(ISR_REGISTER_ADDRESS, &register_value); // reset interrupt
			while(digitalRead(IRQ_PIN) && count < 3000)
				count++;
		}

		readRegister(ISR_REGISTER_ADDRESS, &register_value); // reset interrupt
		count = 0;
		usleep(100000);

//		count = 0;
//		if((!digitalRead(BUSY_FLAG_PIN)))
//			while(!digitalRead(BUSY_FLAG_PIN));
//
//		while(digitalRead(BUSY_FLAG_PIN));

	}

	return SUCCESS;

}


void max3107::restartMax()
{
	char *baundrate;
	initMax(baundrate);
	cout << "Max3107 Restarted." << endl;
}


int max3107::readUart(unsigned char *Data, long Size, bool HeaderFlag)
{

	unsigned long long count = 0;
	unsigned char register_value;
	int remain_size;
	int ret;
	long sequence;
	long transmitted_data_size;
	transmitted_data_size = TRANSMISSON_SIZE + 8;
	unsigned char transmitted_data[transmitted_data_size];

	sequence = Size / TRANSMISSON_SIZE;
	if(Size % TRANSMISSON_SIZE != 0)
		sequence++;
	remain_size = Size % TRANSMISSON_SIZE;

	transmitted_data[0] = UART_RECIEVER_HOLD_REGISTER;

	for(long i=0; i<sequence; i++)
	{
		transmitted_data[0] = UART_RECIEVER_HOLD_REGISTER;

		if(!HeaderFlag)
		{
			do{
				count++;
			}while(digitalRead(IRQ_PIN));
		}
		count = 0;

		readRegister(ISR_REGISTER_ADDRESS, &register_value); // reset interrupt

		ret = gSpi->SpiWriteAndRead(gSpiDevice, &transmitted_data[0], transmitted_data_size);
		if(ret < 0)
			printf("SpiWrite Error!");


		printf("Sequence%d: ", i);
		for(int k=1; k<TRANSMISSON_SIZE; k++)
			printf("%02X-", transmitted_data[k]);
		printf("\n\n");

		if(sequence - 1 == i)
			memcpy(&Data[i*TRANSMISSON_SIZE], &transmitted_data[1], remain_size);
		else
			memcpy(&Data[i*TRANSMISSON_SIZE], &transmitted_data[1], TRANSMISSON_SIZE);

		do{
			count++;
		}while(digitalRead(IRQ_PIN) && count < 2500);
		count = 0;
		readRegister(ISR_REGISTER_ADDRESS, &register_value); // reset interrupt

	}

	return SUCCESS;

}




void max3107::sendRandom()
{

}



void max3107::sendMessage()
{
	string input_string;
	uint64_t size;
	char *data_header = "sendingm";
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];
	unsigned char *header = new unsigned char[2*TRANSMISSION_HEADER_SIZE];
	unsigned char *data;

	cout << "+Enter Message: " << endl;
	cout << "> ";
	cin >> input_string;

	size = input_string.length();

	sprintf((char *)data_size, "%"PRId64"", size);


	gTxBussyFlag = true;


	memcpy(&header[0], data_header, TRANSMISSION_HEADER_SIZE);
	memcpy(&header[TRANSMISSION_HEADER_SIZE], data_size, TRANSMISSION_HEADER_SIZE);
	writeUart(header, 2*TRANSMISSION_HEADER_SIZE);
	delete header;

	data = new unsigned char[size];
	memcpy(data, (unsigned char *)input_string.c_str(), size);
	writeUart(data, size);
	delete data;

	gTxBussyFlag = false;

}



void max3107::listenPort()
{
	if(gListeningFlag == false)
	{
		printf("+Listening the Uart Port...\n");

		thread listen_thread(&max3107::checkData, this);
		listen_thread.detach();
		gListeningFlag = true;
	}
	else
	{
		printf("+The Port is already listening.\n");
	}
}

void max3107::checkData()
{
	unsigned char register_value;
	unsigned char readed_data[TRANSMISSON_SIZE];
	unsigned char data_header[TRANSMISSION_HEADER_SIZE];
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];
	int size;
	int ret;
	int count = 0;

	memset(&readed_data[0], 0, TRANSMISSON_SIZE);
	while(true)
	{

		if(gTxBussyFlag != true)
		{

			ret = digitalRead(IRQ_PIN);

//			if((register_value & RX_INTERRUPT) == RX_INTERRUPT)
			if(ret == 0)
			{

				readUart(&readed_data[0], 2*TRANSMISSION_HEADER_SIZE, 1);

				memcpy(&data_header[0], &readed_data[0], TRANSMISSION_HEADER_SIZE);
				memcpy(&data_size[0], &readed_data[TRANSMISSION_HEADER_SIZE], TRANSMISSION_HEADER_SIZE);

				printf("Data Header: ");
				for(int i=0; i<TRANSMISSION_HEADER_SIZE; i++)
					printf("%c", data_header[i]);
				printf("\n");

				if(strncmp((char*)data_header, "sendingm", TRANSMISSION_HEADER_SIZE) == 0)
				{
					size = stoll((char *)data_size);
					readMessage(size);
	//				for(int i = 0; i<size+1; i++)
	//					usleep(UART_WAITING_TIME);
					memset(&readed_data[0], 0, TRANSMISSON_SIZE);
				}
				else if(strncmp((char *)data_header, "sendingf", TRANSMISSION_HEADER_SIZE) == 0)
				{
					size = stoll((char *)data_size);
					readFile(size);
	//				for(int i = 0; i<size+1; i++)
	//					usleep(UART_WAITING_TIME);
					memset(&readed_data[0], 0, TRANSMISSON_SIZE);
				}
			}
		}
	}
}


int max3107::readMessage(long Size)
{

	unsigned char data[Size];

	printf("Message Size: %d\n", Size);

	readUart(data, Size, 0);

	printf("Message: ");
	for(int i=0; i<Size; i++)
		printf("%c", data[i]);
	printf("\n");

	return SUCCESS;

}

int max3107::readFile(long Size)
{
	long time0;
	long time1;
	unsigned char file_buffer[Size];
	FILE* fout;

	printf("File Size: %d\n", Size);


	time0 = time(NULL);
	readUart(&file_buffer[0], Size, 0);
	time1 = time(NULL);

	printf("Time: %d\n", time1 - time0);

	fout = fopen("recieved_file", "wb");
	if(fout != NULL)
	{
		fwrite(&file_buffer[0], sizeof(unsigned char *), Size, fout);
		fclose(fout);
	}
	else
	{
		printf("+ERROR! The File can not created\n");
		fclose(fout);
		return FAIL;
	}

	return SUCCESS;
}


int max3107::sendFile()
{
	FILE *pFile;

	uint64_t lSize;

	unsigned char *buffer;

	char *data_header = "sendingf";
	unsigned char data_size[TRANSMISSION_HEADER_SIZE];
	unsigned char header[2*TRANSMISSION_HEADER_SIZE];

	size_t ret;

	string file_name;

	cout << "+Please enter File Locatiton" << endl;
	cout << "> ";
	cin >> file_name;

	if(file_name == "1")
		file_name = "/home/pi/file1";
	if(file_name == "0")
		file_name = "/home/pi/file";

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

	gTxBussyFlag = true;

	memcpy(&header[0], data_header, TRANSMISSION_HEADER_SIZE);
	memcpy(&header[TRANSMISSION_HEADER_SIZE], data_size, TRANSMISSION_HEADER_SIZE);
	writeUart(header, 2*TRANSMISSION_HEADER_SIZE);


	writeUart(buffer, lSize);

	gTxBussyFlag = false;
	// terminate
	fclose (pFile);
	free (buffer);

}
