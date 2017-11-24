/*
 * max3107.h
 *
 *  Created on: Oct 23, 2017
 *      Author: root
 */




#ifndef MAX3107_H_
#define MAX3107_H_

#include "libSpi.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <thread>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <sys/time.h>
#include <wiringPi.h>



#define TRANSMISSON_SIZE 88
#define TRANSMISSION_HEADER_SIZE	 8
#define UART_WAITING_TIME 			 132000

#define SUCCESS 0
#define FAIL 	-1

/* Raspberry Pi GPIO */
#define POWER_PIN 0
#define IRQ_PIN 1
#define BUSY_PIN 2
#define BUSY_FLAG_PIN 4

/* Serial error status definitions */
#define PARITY_ERROR	1
#define FRAME_ERROR		2
#define OVERRUN_ERROR	4
#define ALL_ERRORS	(PARITY_ERROR | \
				 FRAME_ERROR | \
				 OVERRUN_ERROR)

/* GPIO definitions */
#define GPIO_BASE	88
#define GPIO_COUNT	4


/* GPIO connected to chip's reset pin */
#define RESET_GPIO	87


/* Chip reset delay */
#define RESET_DELAY	10

/* Chip wakeup delay */
#define WAKEUP_DELAY	50


/* Sleep mode definitions */
#define DISABLE_FORCED_SLEEP	0
#define ENABLE_FORCED_SLEEP		1
#define DISABLE_AUTOSLEEP		2
#define ENABLE_AUTOSLEEP		3

/* Definitions for register access with SPI transfers
 *
 * SPI transfer format:
 *
 * Master to slave bits xzzzzzzzyyyyyyyy
 * Slave to master bits aaaaaaaabbbbbbbb
 *
 * where:
 * x = 0 for reads, 1 for writes
 * z = register address
 * y = new register value if write, 0 if read
 * a = unspecified
 * b = register value if read, unspecified if write
 */

#define RX_INTERRUPT		(1 << 3)
#define TX_INTERRUPT 		(1 << 4)
#define TX_EMPTY_INTERRUPT  (1 << 5)
#define RX_EMPTY_INTERRUPT	(1 << 6)

/* SPI speed */
#define SPI_SPEED	(3125000 * 2)

/* Write bit */
#define WRITE_BIT	(1 << 7)

/* SPI TX data mask */
#define SPI_RX_DATA_MASK	(0x00ff)

/* SPI RX data mask */
#define SPI_TX_DATA_MASK	(0x00ff)

/* IRQ register bits */
#define IRQ_LSR_BIT		(1 << 0) /* LSR interrupt */
#define IRQ_SPCHR_BIT	(1 << 1) /* Special char interrupt */
#define IRQ_STS_BIT		(1 << 2) /* Status interrupt */
#define IRQ_RXFIFO_BIT	(1 << 3) /* RX FIFO interrupt */
#define IRQ_TXFIFO_BIT	(1 << 4) /* TX FIFO interrupt */
#define IRQ_TXEMPTY_BIT	(1 << 5) /* TX FIFO empty interrupt */
#define IRQ_RXEMPTY_BIT	(1 << 6) /* RX FIFO empty interrupt */
#define IRQ_CTS_BIT		(1 << 7) /* CTS interrupt */

/* LSR register bits */
#define LSR_RXTO_BIT	(1 << 0) /* RX timeout */
#define LSR_RXOVR_BIT	(1 << 1) /* RX overrun */
#define LSR_RXPAR_BIT	(1 << 2) /* RX parity error */
#define LSR_FRERR_BIT	(1 << 3) /* Frame error */
#define LSR_RXBRK_BIT	(1 << 4) /* RX break */
#define LSR_RXNOISE_BIT	(1 << 5) /* RX noise */
#define LSR_UNDEF6_BIT	(1 << 6) /* Undefined/not used */
#define LSR_CTS_BIT		(1 << 7) /* CTS pin state */

/* Special character register bits */
#define SPCHR_XON1_BIT		(1 << 0) /* XON1 character */
#define SPCHR_XON2_BIT		(1 << 1) /* XON2 character */
#define SPCHR_XOFF1_BIT		(1 << 2) /* XOFF1 character */
#define SPCHR_XOFF2_BIT		(1 << 3) /* XOFF2 character */
#define SPCHR_BREAK_BIT		(1 << 4) /* RX break */
#define SPCHR_MULTIDROP_BIT	(1 << 5) /* 9-bit multidrop addr char */
#define SPCHR_UNDEF6_BIT	(1 << 6) /* Undefined/not used */
#define SPCHR_UNDEF7_BIT	(1 << 7) /* Undefined/not used */

/* Status register bits */
#define STS_GPIO0_BIT		(1 << 0) /* GPIO 0 interrupt */
#define STS_GPIO1_BIT		(1 << 1) /* GPIO 1 interrupt */
#define STS_GPIO2_BIT		(1 << 2) /* GPIO 2 interrupt */
#define STS_GPIO3_BIT		(1 << 3) /* GPIO 3 interrupt */
#define STS_UNDEF4_BIT		(1 << 4) /* Undefined/not used */
#define STS_CLKREADY_BIT	(1 << 5) /* Clock ready */
#define STS_SLEEP_BIT		(1 << 6) /* Sleep interrupt */
#define STS_UNDEF7_BIT		(1 << 7) /* Undefined/not used */

/* MODE1 register bits */
#define MODE1_RXDIS_BIT			(1 << 0) /* RX disable */
#define MODE1_TXDIS_BIT			(1 << 1) /* TX disable */
#define MODE1_TXHIZ_BIT			(1 << 2) /* TX pin three-state */
#define MODE1_RTSHIZ_BIT		(1 << 3) /* RTS pin three-state */
#define MODE1_TRNSCVCTRL_BIT	(1 << 4) /* Transceiver ctrl enable */
#define MODE1_FORCESLEEP_BIT	(1 << 5) /* Force sleep mode */
#define MODE1_AUTOSLEEP_BIT		(1 << 6) /* Auto sleep enable */
#define MODE1_IRQSEL_BIT		(1 << 7) /* IRQ pin enable */

/* MODE2 register bits */
#define MODE2_RST_BIT		(1 << 0) /* Chip reset */
#define MODE2_FIFORST_BIT	(1 << 1) /* FIFO reset */
#define MODE2_RXTRIGINV_BIT	(1 << 2) /* RX FIFO INT invert */
#define MODE2_RXEMPTINV_BIT	(1 << 3) /* RX FIFO empty INT invert */
#define MODE2_SPCHR_BIT		(1 << 4) /* Special chr detect enable */
#define MODE2_LOOPBACK_BIT	(1 << 5) /* Internal loopback enable */
#define MODE2_MULTIDROP_BIT	(1 << 6) /* 9-bit multidrop enable */
#define MODE2_ECHOSUPR_BIT	(1 << 7) /* ECHO suppression enable */

/* LCR register bits */
#define LCR_LENGTH0_BIT		(1 << 0) /* Word length bit 0 */
#define LCR_LENGTH1_BIT		(1 << 1) /* Word length bit 1
						  *
						  * Word length bits table:
						  * 00 -> 5 bit words
						  * 01 -> 6 bit words
						  * 10 -> 7 bit words
						  * 11 -> 8 bit words
						  */
#define LCR_STOPLEN_BIT		(1 << 2) /* STOP length bit
						  *
						  * STOP length bit table:
						  * 0 -> 1 stop bit
						  * 1 -> 1-1.5 stop bits if
						  *      word length is 5,
						  *      2 stop bits otherwise
						  */
#define LCR_PARITY_BIT		(1 << 3) /* Parity bit enable */
#define LCR_EVENPARITY_BIT	(1 << 4) /* Even parity bit enable */
#define LCR_FORCEPARITY_BIT	(1 << 5) /* 9-bit multidrop parity */
#define LCR_TXBREAK_BIT		(1 << 6) /* TX break enable */
#define LCR_RTS_BIT			(1 << 7) /* RTS pin control */
#define LCR_WORD_LEN_5		(0x0000)
#define LCR_WORD_LEN_6		(0x0001)
#define LCR_WORD_LEN_7		(0x0002)
#define LCR_WORD_LEN_8		(0x0007)

/* IRDA register bits */
#define IRDA_IRDAEN_BIT		(1 << 0) /* IRDA mode enable */
#define IRDA_SIR_BIT		(1 << 1) /* SIR mode enable */
#define IRDA_SHORTIR_BIT	(1 << 2) /* Short SIR mode enable */
#define IRDA_MIR_BIT		(1 << 3) /* MIR mode enable */
#define IRDA_RXINV_BIT		(1 << 4) /* RX logic inversion enable */
#define IRDA_TXINV_BIT		(1 << 5) /* TX logic inversion enable */
#define IRDA_UNDEF6_BIT		(1 << 6) /* Undefined/not used */
#define IRDA_UNDEF7_BIT		(1 << 7) /* Undefined/not used */

/* Flow control trigger level register masks */
#define FLOWLVL_HALT_MASK	(0x000f) /* Flow control halt level */
#define FLOWLVL_RES_MASK	(0x0000) /* Flow control resume level */
#define FLOWLVL_HALT(words)	((words/8) & 0x000f)
#define FLOWLVL_RES(words)	(((words/8) & 0x000f) << 4)

/* FIFO interrupt trigger level register masks */
#define FIFOTRIGLVL_TX_MASK		(0x000f) /* TX FIFO trigger level */
#define FIFOTRIGLVL_RX_MASK		(0x00f0) /* RX FIFO trigger level */
#define FIFOTRIGLVL_TX(words)	((words/8) & 0x000f)
#define FIFOTRIGLVL_RX(words)	(((words/8) & 0x000f) << 4)

/* Flow control register bits */
#define FLOWCTRL_AUTORTS_BIT	(1 << 0) /* Auto RTS flow ctrl enable */
#define FLOWCTRL_AUTOCTS_BIT	(1 << 1) /* Auto CTS flow ctrl enable */
#define FLOWCTRL_GPIADDR_BIT	(1 << 2) /* Enables that GPIO inputs
						  * are used in conjunction with
						  * XOFF2 for definition of
						  * special character */
#define FLOWCTRL_SWFLOWEN_BIT	(1 << 3) /* Auto SW flow ctrl enable */
#define FLOWCTRL_SWFLOW0_BIT	(1 << 4) /* SWFLOW bit 0 */
#define FLOWCTRL_SWFLOW1_BIT	(1 << 5) /* SWFLOW bit 1
						  *
						  * SWFLOW bits 1 & 0 table:
						  * 00 -> no transmitter flow
						  *       control
						  * 01 -> receiver compares
						  *       XON2 and XOFF2
						  *       and controls
						  *       transmitter
						  * 10 -> receiver compares
						  *       XON1 and XOFF1
						  *       and controls
						  *       transmitter
						  * 11 -> receiver compares
						  *       XON1, XON2, XOFF1 and
						  *       XOFF2 and controls
						  *       transmitter
						  */
#define FLOWCTRL_SWFLOW2_BIT	(1 << 6) /* SWFLOW bit 2 */
#define FLOWCTRL_SWFLOW3_BIT	(1 << 7) /* SWFLOW bit 3
						  *
						  * SWFLOW bits 3 & 2 table:
						  * 00 -> no received flow
						  *       control
						  * 01 -> transmitter generates
						  *       XON2 and XOFF2
						  * 10 -> transmitter generates
						  *       XON1 and XOFF1
						  * 11 -> transmitter generates
						  *       XON1, XON2, XOFF1 and
						  *       XOFF2
						  */

/* GPIO configuration register bits */
#define GPIOCFG_GP0OUT_BIT	(1 << 0) /* GPIO 0 output enable */
#define GPIOCFG_GP1OUT_BIT	(1 << 1) /* GPIO 1 output enable */
#define GPIOCFG_GP2OUT_BIT	(1 << 2) /* GPIO 2 output enable */
#define GPIOCFG_GP3OUT_BIT	(1 << 3) /* GPIO 3 output enable */
#define GPIOCFG_GP0OD_BIT	(1 << 4) /* GPIO 0 open-drain enable */
#define GPIOCFG_GP1OD_BIT	(1 << 5) /* GPIO 1 open-drain enable */
#define GPIOCFG_GP2OD_BIT	(1 << 6) /* GPIO 2 open-drain enable */
#define GPIOCFG_GP3OD_BIT	(1 << 7) /* GPIO 3 open-drain enable */

/* GPIO DATA register bits */
#define GPIODATA_GP0OUT_BIT	(1 << 0) /* GPIO 0 output value */
#define GPIODATA_GP1OUT_BIT	(1 << 1) /* GPIO 1 output value */
#define GPIODATA_GP2OUT_BIT	(1 << 2) /* GPIO 2 output value */
#define GPIODATA_GP3OUT_BIT	(1 << 3) /* GPIO 3 output value */
#define GPIODATA_GP0IN_BIT	(1 << 4) /* GPIO 0 input value */
#define GPIODATA_GP1IN_BIT	(1 << 5) /* GPIO 1 input value */
#define GPIODATA_GP2IN_BIT	(1 << 6) /* GPIO 2 input value */
#define GPIODATA_GP3IN_BIT	(1 << 7) /* GPIO 3 input value */

/* PLL configuration register masks */
#define PLLCFG_PREDIV_MASK		(0x003f) /* PLL predivision value */
#define PLLCFG_PLLFACTOR_MASK	(0x00c0) /* PLL multiplication factor */
#define PLLCFG_FACTOR0			(1 << 6)
#define PLLCFG_FACTOR1			(1 << 7)

/* Baud rate generator configuration register masks and bits */
#define BRGCFG_FRACT_MASK	(0x000f) /* Fractional portion of
						  * Baud rate generator divisor
						  */
#define BRGCFG_2XMODE_BIT	(1 << 4) /* Double baud rate */
#define BRGCFG_4XMODE_BIT	(1 << 5) /* Quadruple baud rate */
#define BRGCFG_UNDEF6_BIT	(1 << 6) /* Undefined/not used */
#define BRGCFG_UNDEF7_BIT	(1 << 7) /* Undefined/not used */

/* Clock source register bits */
#define CLKSRC_INTOSC_BIT	(1 << 0) /* Internal osc enable */
#define CLKSRC_CRYST_BIT	(1 << 1) /* Crystal osc enable */
#define CLKSRC_PLL_BIT		(1 << 2) /* PLL enable */
#define CLKSRC_PLLBYP_BIT	(1 << 3) /* PLL bypass */
#define CLKSRC_EXTCLK_BIT	(1 << 4) /* External clock enable */
#define CLKSRC_UNDEF5_BIT	(1 << 5) /* Undefined/not used */
#define CLKSRC_UNDEF6_BIT	(1 << 6) /* Undefined/not used */
#define CLKSRC_CLK2RTS_BIT	(1 << 7) /* Baud clk to RTS pin */


/* HW definitions */
//#define RX_FIFO_SIZE	128
//#define TX_FIFO_SIZE	128
#define REVID1		0x00a0
#define REVID2		0x00a1


/* Baud rate generator configuration values for external clock 13MHz */
#define BRG13_B300		(0x0A9400 | 0x05)
#define BRG13_B600		(0x054A00 | 0x03)
#define BRG13_B1200		(0x02A500 | 0x01)
#define BRG13_B2400		(0x015200 | 0x09)
#define BRG13_B4800		(0x00A900 | 0x04)
#define BRG13_B9600		(0x005400 | 0x0A)
#define BRG13_B19200	(0x002A00 | 0x05)
#define BRG13_B38400	(0x001500 | 0x03)
#define BRG13_B57600	(0x000E00 | 0x02)
#define BRG13_B115200	(0x000700 | 0x01)
#define BRG13_B230400	(0x000300 | 0x08)
#define BRG13_B460800	(0x000100 | 0x0c)
#define BRG13_B921600	(0x000100 | 0x1c)

/* Baud rate generator configuration values for external clock 26MHz */
#define BRG26_B300		(0x152800 | 0x0A)
#define BRG26_B600		(0x0A9400 | 0x05)
#define BRG26_B1200		(0x054A00 | 0x03)
#define BRG26_B2400		(0x02A500 | 0x01)
#define BRG26_B4800		(0x015200 | 0x09)
#define BRG26_B9600		(0x00A900 | 0x04)
#define BRG26_B19200	(0x005400 | 0x0A)
#define BRG26_B38400	(0x002A00 | 0x05)
#define BRG26_B57600	(0x001C00 | 0x03)
#define BRG26_B115200	(0x000E00 | 0x02)
#define BRG26_B230400	(0x000700 | 0x01)
#define BRG26_B460800	(0x000300 | 0x08)
#define BRG26_B921600	(0x000100 | 0x0C)

/* Baud rate generator configuration values for external clock 8MHZ */
#define MHZ8_B9600		(0x003400 | 0x01)
#define MHZ8_B38400 	(0x000D00 | 0x00)
#define MHZ8_B57600		(0x000800 | 0x0B)
#define MHZ8_B115200	(0x000400 | 0x05)
#define MHZ8_B230400	(0x000200 | 0x03)
#define MHZ8_B265000	(0x000100 | 0x0E)
#define MHZ8_B276000 	(0x000100 | 0x0D)
#define MHZ8_B285000	(0x000100 | 0x0C)
#define MHZ8_B300000	(0x000100 | 0x0B)
#define MHZ8_B308000	(0x000100 | 0x0A)
#define MHZ8_B320102	(0x000100 | 0x09)
#define MHZ8_B333000	(0x000100 | 0x08)
#define MHZ8_B348000	(0x000100 | 0x07)
#define MHZ8_B364000	(0x000100 | 0x06)
#define MHZ8_B381000	(0x000100 | 0x05)
#define MHZ8_B400000	(0x000100 | 0x04)
#define MHZ8_B421000	(0x000100 | 0x03)
#define MHZ8_B444000	(0x000100 | 0x02)
#define MHZ8_B460800	(0x000100 | 0x01)
#define MHZ8_B500000	(0x000100 | 0x20)
#define MHZ8_B921600	(0x000100 | 0x11) // for 2x mode
#define MHZ8_B1000000 	(0x000200 | 0x20) // for 4x mode
#define MHZ8_B2000000	(0x000100 | 0x20) // for 4x mode

/* Baud rate generator configuration values for external clock 20MHZ */
#define MHZ20_B9600		(0x008200 | 0x03)
#define MHZ20_B38400 	(0x002000 | 0x09)
#define MHZ20_B57600	(0x001500 | 0x0B)
#define MHZ20_B115200	(0x000A00 | 0x0E)
#define MHZ20_B230400	(0x000500 | 0x07)
#define MHZ20_B460800	(0x000200 | 0x0B)
#define MHZ20_B921600	(0x000100 | 0x06)
#define MHZ20_B1000000 	(0x000200 | 0x11) // for 2x mode
#define MHZ20_B2000000	(0x000200 | 0x22) // for 4x mode
#define MHZ20_B3000000 	(0x000100 | 0x23) // for 4x mode
#define MHZ20_B4000000	(0x000100 | 0x21) // for 4x mode


/* Baud rate generator configuration values for internal clock 614.4KHZ*/
#define IB9600		(0x0400 | 0x00)
#define IB38400		(0x0100 | 0x00)


#define UART_TRANSMIT_HOLD_REGISTER 		0x00
#define UART_RECIEVER_HOLD_REGISTER		 	0x00
#define IRQEN_REGISTER_ADDRESS 				0x01
#define ISR_REGISTER_ADDRESS 				0x02
#define LSRINTEN_REGISTER_ADDRESS 			0x03
#define LSR_REGISTER_ADDRESS 				0x04
#define SPCLCHRINTEN__REGISTER_ADDRESS 		0x05
#define STSINTEN_REGISTER_ADDRESS 			0x07
#define STSINT_REGISTER_ADDRESS 			0x08
#define MODE1_REGISTER_ADDRESS 				0x09
#define MODE2_REGISTER_ADDRESS				0x0A
#define LCR_REGISTER_ADDRESS 				0x0B
#define RXTIMEOUT_REGISTER_ADDRESS 			0x0C
#define HDPLXDELAY_REGISTER_ADDRESS 		0x0D
#define IRDA_REGISTER_ADDRESS 				0x0E
#define FLOWLVL_REGISTER_ADDRESS			0x0F
#define FIFOTRGLVL_REGISTER_ADDRESS 		0x10
#define TXFIFOLVL_REGISTER_ADDRESS 			0x11
#define RXFIFOLVL_REGISTER_ADDRESS			0x12
#define FLOWCTRL_REGISTER_ADDRESS			0x13
#define XON1_REGISTER_ADDRESS				0x14
#define XON2_REGISTER_ADDRESS				0x15
#define XOFF1_REGISTER_ADDRESS				0x16
#define XOFF2_REGISTER_ADDRESS				0x17
#define GPIOCONFIG_REGISTER_ADDRESS 		0x18
#define GPIODATA_REGISTER_ADDRESS 			0x19
#define PLLCONFIG_REGISTER_ADDRESS			0x1A
#define BRGCONFIG_REGISTER_ADDRESS			0x1B
#define DIVLSB_REGISTER_ADDRESS				0x1C
#define DIVMSB_REGISTER_ADDRESS				0x1D
#define CLKSOURCE_REGISTER_ADDRESS			0x1E
#define REVID_REGISTER_ADDRESS				0X1F


class max3107 {
public:
	max3107(int SpiDevice, long SpiSpeed);
	virtual ~max3107();

	int initMax(char *BaundRate);
	int writeRegister(int RegisterAddress, int Value);
	int readRegister(int RegisterAddress, unsigned char *RegisterValue);

	void sendRandom();
	void sendMessage();
	int sendFile();
	void listenPort();
	void checkData();
	void restartMax();
	void resetFifo();

	int readMessage(long Size);
	int readFile(long Size);
	int writeUart(unsigned char *Data, long Size);
	int readUart(unsigned char *Data, long Size, bool HeaderFlag);


	bool gListeningFlag = false;
	bool gTxBussyFlag = false;
	int gSpiDevice;
	libSpi *gSpi;



};

#endif /* MAX3107_H_ */
