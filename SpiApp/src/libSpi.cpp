/*
 * libSpi.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: root
 */

#include "libSpi.h"

libSpi::libSpi() {
	// TODO Auto-generated constructor stub

}

libSpi::~libSpi() {
	// TODO Auto-generated destructor stub
}



//***********************************
//***********************************
//********** SPI OPEN PORT **********
//***********************************
//***********************************
//spi_device	0=CS0, 1=CS1
int libSpi::SpiOpenPort(int spi_device, long speed)
{
	int status_value = -1;
	int *spi_cs_fd;



	//----- SET SPI MODE -----
	//SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	//SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	//SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
	//SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
	gSpiMode = SPI_MODE_0;

	//----- SET BITS PER WORD -----
	gSpiPerWords = 8;

	//----- SET SPI BUS SPEED -----
//	spi_speed = 1000000;		//1000000 = 1MHz (1uS per bit)
	gSpiSpeed = speed;

	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	if (spi_device)
		*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
	else
		*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

	if (*spi_cs_fd < 0)
	{
		perror("Error - Could not open SPI device");
		exit(1);
	}
	gSpiDevice = spi_device;

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &gSpiMode);
	if(status_value < 0)
	{
		perror("Could not set SPIMode (WR)...ioctl fail");
		exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &gSpiMode);
	if(status_value < 0)
	{
	  perror("Could not set SPIMode (RD)...ioctl fail");
	  exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &gSpiPerWords);
	if(status_value < 0)
	{
	  perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
	  exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &gSpiPerWords);
	if(status_value < 0)
	{
	  perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
	  exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &gSpiSpeed);
	if(status_value < 0)
	{
	  perror("Could not set SPI speed (WR)...ioctl fail");
	  exit(1);
	}

	status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &gSpiSpeed);
	if(status_value < 0)
	{
	  perror("Could not set SPI speed (RD)...ioctl fail");
	  exit(1);
	}
	return(status_value);
}



//************************************
//************************************
//********** SPI CLOSE PORT **********
//************************************
//************************************
int libSpi::SpiClosePort(int spi_device)
{
	int status_value = -1;
	int *spi_cs_fd;

	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;


	status_value = close(*spi_cs_fd);
	if(status_value < 0)
	{
		perror("Error - Could not close SPI device");
		exit(1);
	}
	return(status_value);
}



//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int libSpi::SpiWriteAndRead(int spi_device, unsigned char* TransmitedData, int length)
{
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int retVal = -1;
	int *spi_cs_fd;

	if (spi_device)
		spi_cs_fd = &spi_cs1_fd;
	else
		spi_cs_fd = &spi_cs0_fd;

	//one spi transfer for each byte

	for (i = 0 ; i < length ; i++)
	{
		memset(&spi[i], 0, sizeof (spi[i]));
		spi[i].tx_buf        = (unsigned long)(TransmitedData + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(TransmitedData + i) ; // receive into "data"
		spi[i].len           = sizeof(*(TransmitedData + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = gSpiSpeed ;
		spi[i].bits_per_word = gSpiPerWords ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}
