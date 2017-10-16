/*
 * MCP2515.c
 *
 * Created: 06.10.2017 11:02:08
 *  Author: danieta
 */ 
//#define F_CPU 4915200 Denne skal være unødvendig, kommenterer ut i tilfelle (er nå på node 2)
#include <avr/io.h>
#include <util/delay.h>
#include "MCP2515.h"
#include "spi.h"

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))

uint8_t mcp2515_read(uint8_t address){
	clear_bit(PORTB, SS); //Setter SS lav

	uint8_t result;
	spi_MasterTransmit(MCP_READ);
	spi_MasterTransmit(address);
	result = spi_MasterRead(); //Leser hva som var i adressen/registeret vi aksesserte

	set_bit(PORTB, SS); //Setter SS høy
	
	return result;
}

void mcp2515_write(uint8_t address, uint8_t data){
	clear_bit(PORTB,SS); //Setter SS lav
	
	spi_MasterTransmit(MCP_WRITE); //Velger instruction
	
	spi_MasterTransmit(address); //Velger adresse, f.eks. 0b00110001
	
	spi_MasterTransmit(data); //Velger data som sendes

	set_bit(PORTB,SS);
}

void mcp2515_reset(){
	clear_bit(PORTB,SS); //Setter SS lav
	spi_MasterTransmit(MCP_RESET);
	set_bit(PORTB,SS); //Setter SS høy
}

uint8_t mcp2515_init(){
	uint8_t value;
	spi_init(); //Initialize SPI
	mcp2515_reset(); // Send reset-command
	_delay_us(8); //oscillatoren er i reset modus i 128 klokkesykluser. trenger derav en delay
	
	//Self-test to check if system is in config mode
	value = mcp2515_read(MCP_CANSTAT); 
	if((value & MODE_MASK)  != MODE_CONFIG) {
		return 1;
	}
	// More initialization
	return 0;
}

void mcp2515_request_to_send(int whichBuffer){ //se s. 66
	clear_bit(PORTB,SS); //Setter SS lav

	if(whichBuffer == 0){ //choose TXB0
		spi_MasterTransmit(0b10000001); 
	}
	
	else if (whichBuffer == 1){ //choose TXB1
		spi_MasterTransmit(0b10000010); 
	}
	else{ //choose TXB2
		spi_MasterTransmit(0b10000100);
	}
	
	set_bit(PORTB,SS); //Setter SS høy
}

uint8_t mcp2515_read_status(){
	clear_bit(PORTB,SS); //Setter SS lav
	
	uint8_t statusRegister;
	uint8_t statusRegister2;

	spi_MasterTransmit(MCP_READ_STATUS);
	statusRegister = spi_MasterRead(); //disse to skal være like hverandre, se s. 69
	statusRegister2 = spi_MasterRead(); //disse to skal være like hverandre, se s. 69

	if(statusRegister != statusRegister2){
		printf("Status registers in mcp2515_read_status() not identical.");
		return 0;
	}

	set_bit(PORTB,SS); //Setter SS høy

	return statusRegister;
}


void mcp2515_bit_modify(uint8_t regAdr, uint8_t maskBits, uint8_t data){ //se s. 68
	clear_bit(PORTB,SS); //Setter SS lav

	spi_MasterTransmit(MCP_BITMOD);
	spi_MasterTransmit(regAdr);
	spi_MasterTransmit(maskBits);
	spi_MasterTransmit(data);

	set_bit(PORTB,SS); //Setter SS høy
}

void loadTxBuffer(uint16_t identifier, uint8_t *data, uint8_t lengthOfData){
	clear_bit(PORTB, SS);
	
	// Check that lengthOfData is not bigger than 8
	
	spi_MasterTransmit(MCP_LOAD_TX0);
	//spi_MasterTransmit(first 8 of id)
	//spi_MasterTransmitt(last bits id)
	
	set_bit(PORTB, SS);
	clear_bit(PORTB, SS);
	spi_MasterTransmit(MCP_LOAD_TX0 + 1);
	for(int i = 0; i < lengthOfData; i++){
		spi_MasterTransmit(data[i]);
	}
	clear_bit(PORTB, SS);
}
