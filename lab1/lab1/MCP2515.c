/*
 * MCP2515.c
 *
 * Created: 06.10.2017 11:02:08
 *  Author: danieta
 */ 
#include "MCP2515.h"
/*
uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	
	PORTB &= ~(1<<CAN_CS); //Select CAN-controller
	SPI_write(MCP_READ); //Send read command
	SPI_write(address); //Send address
	result = SPI_read(); //Read result
	PORTB |= (1<<CAN_CS); //Deselect CAN-controller
	
	return result;
}

uint8_t mcp2515_init(){
	uint8_t value;
	SPI_init(); //Initialize SPI /////////////////////må ha en SPI_init funksjon i så fall
	mcp2515_reset(); // Send reset-command
	
	//Self-test
	mcp2515_read(MCP_CANSTAT, &value);
	if((value & MODE_MASK)  != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration	mode after reset!\n");
		return 1;
	}
	// More initialization
	return 0;
}


uint8_t readByteFromMcp(uint8_t address){
	SPI_SEND(0b00000011);
	SPI_SEND(address);
	SPI_SEND(DC);
	return SPDR;
}

*/