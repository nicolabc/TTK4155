/*
 * spi.h
 *
 * Created: 02.10.2017 15:52:39
 *  Author: danieta
 */ 


#ifndef SPI_H_
#define SPI_H_


void spi_MasterInit(void);
uint8_t spi_MasterTransmit(char cData, uint8_t keepLow); //void spi_send();
uint8_t spi_MasterRead(uint8_t mcp_address);



#endif /* SPI_H_ */