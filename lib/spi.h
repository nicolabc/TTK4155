/*
 * spi.h
 *
 * Created: 02.10.2017 15:52:39
 *  Author: danieta
 */ 


#ifndef SPI_H_
#define SPI_H_

#if __AVR_ATmega162__
	#define SS PB4
	#define MOSI PB5
	#define MISO PB6
	#define SCK PB7

#elif __AVR_ATmega2560__
	#define SS PB7
	#define MOSI PB2
	#define MISO PB3
	#define SCK PB1
	
#endif

//SS, MOSI, MISO, SCK   PB7, ikke pb4 er ss
//sett PB0 til output og sett pb0 høy, miso lav, 

/*Set relevant pins to input/output and enable SPI bits (SS, MOSI, MISO and SCK*/
void spi_init(void);

/*Sends data from the master*/
void spi_MasterTransmit(char cData); 

/*Reads data to the master*/
uint8_t spi_MasterRead();


#endif /* SPI_H_ */