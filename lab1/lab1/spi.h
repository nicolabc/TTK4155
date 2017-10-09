/*
 * spi.h
 *
 * Created: 02.10.2017 15:52:39
 *  Author: danieta
 */ 


#ifndef SPI_H_
#define SPI_H_


void spi_init(void);
//uint8_t spi_MasterTransmit(char cData, uint8_t keepLow); //void spi_send();
void spi_MasterTransmit(char cData); //void spi_send();
uint8_t spi_MasterRead();




#endif /* SPI_H_ */