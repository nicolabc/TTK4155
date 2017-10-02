/*
 * spi.c
 *
 * Created: 02.10.2017 15:52:20
 *  Author: danieta
 */ 

 #include <avr/io.h>
 #include "spi.h"
 #include "MCP2515.h"
 #define set_bit( reg, bit ) (reg |= (1 << bit))
 #define clear_bit( reg, bit ) (reg &= ~(1 << bit))

 void SPI_MasterInit(void){
     /* Set MOSI and SCK output, all others input */
     DDRB = (1<<DDB5)|(1<<DDB7);
     /* Enable SPI, Master, set clock rate fck/16 */
     SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	 //Sette SS* lav
	 set_bit(DDRB,PB4); //Setter PB4 til write/output
	 clear_bit(PORTB,PB4);

 }
 void SPI_MasterTransmit(char cData) {
     /* Start transmission */
     SPDR = cData;
     /* Wait for transmission complete */
     while(!(SPSR & (1<<SPIF)));
 }