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
    DDRB |= (1<<DDB5)|(1<<DDB7);
	DDRB &= ~(1 << DDB6);
    /* Enable SPI, Master, set clock rate fck/2 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	
	set_bit(DDRB,DDB4); //Setter PB4 til write/output
	set_bit(PORTB, PB4);
	
}

//Med denne må vi sette SS lav/høy før/etter mastertransmit starter/slutter
void spi_MasterTransmit(char cData) { //keepLow holds the SS* signal low so the transmission continues
	
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). */
	while(!(SPSR & (1<<SPIF)));
	
	
}


//ANNEN VERSJON AV MASTERTRANSMIT. VERSJONEN VI BRUKER HAR KEEPLOW, MEN I DENNE MASTERTRANSMIT VILLE VI HELLER SATT SS LAV OG HØY MANUELT FØR VI KALLER PÅ DENNE VERSJONEN
//Torjus som foreslo.
/*
uint8_t spi_MasterTransmit(char cData, uint8_t keepLow) { //keepLow holds the SS* signal low so the transmission continues
	//Sette SS* lav
	clear_bit(PORTB,PB4);
	
	// * Start transmission * /
    SPDR = cData;
    // * Wait for transmission complete. Interrupt flag in SPSR is set (SPIF). * /
    while(!(SPSR & (1<<SPIF)));
	
	if(!keepLow){ //All transmissions are done. Slave is deselected
		 set_bit(PORTB, PB4);
	}
	return SPDR;
	 
}
*/



//Egentlig ikke nødvendig med denne funksjonen, fordi MasterTransmit gjør det samme, men vi beholder den for lesbarhet av koden
uint8_t spi_MasterRead(){
	//Setter SS lav utenfor funksjonen, altså setter vi SS lav inni mcp2515_read

	spi_MasterTransmit(0x00);
	
	//Setter SS høy utenfor funksjonen, altså setter vi SS høy igjen inni mcp2515_read

	return SPDR;
	
}


