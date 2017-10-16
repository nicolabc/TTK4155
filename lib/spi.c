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

void spi_init(void){
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1<<MOSI)|(1<<SCK);
	DDRB &= ~(1 << MISO);
    /* Enable SPI, Master, set clock rate fck/2 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	
	set_bit(DDRB,SS); //Setter PB4 til write/output
	set_bit(PORTB, SS);
	
	if(SS==7){ //Hvis vi har arduino'en, trenger å sette PB0 til output og høy
		set_bit(DDRB,PB0); //Setter PB4 til write/output
		set_bit(PORTB, PB0);
	}
}

//Med denne må vi sette SS lav/høy før/etter mastertransmit starter/slutter
void spi_MasterTransmit(char cData) { //keepLow holds the SS* signal low so the transmission continues
	
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
	while(!(SPSR & (1<<SPIF)));
}


//Egentlig ikke nødvendig med denne funksjonen, fordi MasterTransmit gjør det samme, men vi beholder den for lesbarhet av koden
uint8_t spi_MasterRead(){
	//Setter SS lav utenfor funksjonen, altså setter vi SS lav inni mcp2515_read

	spi_MasterTransmit(0x00);
	
	//Setter SS høy utenfor funksjonen, altså setter vi SS høy igjen inni mcp2515_read

	return SPDR;
	
}


