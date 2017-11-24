/*
 * libSpi.h
 *
 *  Created on: Oct 17, 2017
 *      Author: root
 */

#ifndef LIBSPI_H_
#define LIBSPI_H_

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
#include <cstdint>


class libSpi {
public:
	libSpi();
	virtual ~libSpi();

	int SpiOpenPort(int spi_device, long speed);
	int SpiClosePort (int spi_device);
	int SpiWriteAndRead (int spi_device, unsigned char *data, int length);

	int spi_cs0_fd;				//file descriptor for the SPI device
	int spi_cs1_fd;				//file descriptor for the SPI device
	unsigned char gSpiMode;
	unsigned char gSpiPerWords;
	int gSpiSpeed;
	int gSpiDevice;

};

#endif /* LIBSPI_H_ */
