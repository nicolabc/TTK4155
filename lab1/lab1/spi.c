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

void spi_MasterInit(void){
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1<<DDB5)|(1<<DDB7);
	DDRB &= ~(1 << DDB6);
    /* Enable SPI, Master, set clock rate fck/2 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	
	set_bit(DDRB,DDB4); //Setter PB4 til write/output
	set_bit(PORTB, PB4);
	
}
uint8_t spi_MasterTransmit(char cData, uint8_t keepLow) { //keepLow holds the SS* signal low so the transmission continues
	//Sette SS* lav
	clear_bit(PORTB,PB4);
	
	/* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
    while(!(SPSR & (1<<SPIF)));
	
	if(!keepLow){ //All transmissions are done. Slave is deselected
		 set_bit(PORTB, PB4);
	}
	return SPDR;
	 
}

uint8_t spi_read(){
	return SPDR;
}

uint8_t spi_MasterRead(uint8_t mcp_address){
	//Sette SS* lav
	clear_bit(PORTB,PB4);
	
	/* Start transmission */
	SPDR = MCP_READ; //Choose the instruction read
	/* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
	while(!(SPSR & (1<<SPIF)));
	
	
	/* Start transmission */
	SPDR = mcp_address; //Choose which address to read from
	/* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
	while(!(SPSR & (1<<SPIF)));
	
	
	/* Start transmission */
	SPDR = 0x00; //Don't care
	/* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
	while(!(SPSR & (1<<SPIF)));
	
	
	//All transmissions are done. Slave is deselected
	set_bit(PORTB, PB4);
	
	return SPDR;
	
}
/*
spiSend(0b00000010);
spiSend(0b00110001);
spiSend(data);

spiSend(0b00000011);
spiSend(0b00110001);
spiSend(DC);
data = SPDR;
*/
