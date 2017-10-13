/*
 * lab1.c
 *
 * Created: 04.09.2017 08:51:27
 *  Author: nicolabc
 */ 

#include <stdint.h>
//#include <inttypes.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>


#define F_CPU 4915200

#include "util/delay.h"
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "externalmemory.h"
#include "avr.h"
#include "oled.h"
#include "joy.h"
#include "menu.h"
#include "spi.h"
#include "MCP2515.h"
#include "can.h"
//#include "fonts.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

volatile int RECEIVE_BUFFER_INTERRUPT = 0;

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
}

int main(void)
{
	
	USART_Init(MYUBRR);
	extMem_init(); 
	avr_init();
	adc_init();
	oled_init();
	oled_clear_screen();
	menu_init();
	can_init();
	
	
	
	
	
	while(1)
	{
		can_msg melding;
		melding.id = 5;
		melding.length = 1;
		melding.data[0] = (uint8_t)('Q');
		
		//_delay_ms(3000); //for å sjekke om interrupten blir høy
		
		can_send_message(melding.id, melding.data, melding.length);
		can_msg mottatt;
		
		if(RECEIVE_BUFFER_INTERRUPT){
			mottatt = can_receive_message();
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
			RECEIVE_BUFFER_INTERRUPT = 0; //clearer interruptflagget 
			char mottatt_data_char = mottatt.data[0];
			
			printf("ID: %i  LENGTH: %i   DATA  %c \n", mottatt.id , mottatt.length, mottatt_data_char);
			
		}
		
		//printf("CANSTAT: %x\n",mcp2515_read(MCP_CANSTAT));
		//printf("CANINTF: %x\n",mcp2515_read(MCP_CANINTF));
		
		
		if(joy_doesDirectionChange()){
			menu_save();
			oled_refresh();
		}
	}
	
	
	
	//mcp2515_write(MCP_TXB0SIDH, 0x10);
	/*mcp2515_request_to_send(0);
	while(1){
		uint8_t data;

		data = mcp2515_read(MCP_TXB0CTRL);

		printf("Returnert verdi %02X\n", data); //printer verdien vi leser fra registeret i hex
		_delay_ms(1000);
	}
	
	
	
	*/
	
}

//Neste gang: sjekke med oscilloskop hva som skjer med interrupt-pinen på mcp2515 for å sjekke om RECEIVE_BUFFER_INTERRUPT blir lav igjen. Om den ikke blir lav igjen, må vi fikse på et register eller noe

