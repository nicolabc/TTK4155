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
#include "../../lib/uart.h"
#include "sram.h"
#include "adc.h"
#include "externalmemory.h"
#include "avr.h"
#include "oled.h"
#include "joy.h"
#include "menu.h"
#include "../../lib/spi.h"
#include "../../lib/MCP2515.h"
#include "../../lib/can.h"
//#include "fonts.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

volatile int RECEIVE_BUFFER_INTERRUPT = 0;

volatile char buffer[50];
volatile uint8_t lengthOfBuffer = 0;

int addToBuffer(uint8_t *data, uint8_t sizeOfData){
	if(sizeOfData + lengthOfBuffer > 50){
		return 0;
	}
	memcpy(buffer + lengthOfBuffer, data, sizeOfData);
	lengthOfBuffer += sizeOfData; 
	return 1;
}

int readFromBuffer(uint8_t *data, uint8_t sizeOfData){
	if(sizeOfData < lengthOfBuffer){
		return 0;
	}
	memcpy(data, buffer, lengthOfBuffer);
	lengthOfBuffer = 0;
	return 1;
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
	
	can_msg yourMessage;
	/*melding.id = 6;
	melding.length = 8;
	
	melding.data[0] = (uint8_t)('s');
	melding.data[1] = (uint8_t)('l');
	melding.data[2] = (uint8_t)('e');
	melding.data[3] = (uint8_t)('t');
	melding.data[4] = (uint8_t)('t');
	melding.data[5] = (uint8_t)('d');
	melding.data[6] = (uint8_t)('e');
	melding.data[7] = (uint8_t)('n');
	*/
	//joy_sendMultiboardInfo(&melding);
	
	

	
	while(1)
	{
		yourMessage.id = 1; //Her bestemmer vi at en ID som 1 vil være en melding fra joysticken
		yourMessage.length = 7;
		yourMessage.data[0] = adc_read(0); //sender inn x-verdien til joysticken først. Denne verdien er mellom -100 og 100
		yourMessage.data[1] = adc_read(1); //y-verdien til joystick
		yourMessage.data[2] = joy_readButton(2); //leser joystick-knappen (L3 på ps4)
		yourMessage.data[3] = joy_readButton(0); //leser venstre knapp
		yourMessage.data[4] = joy_readButton(1); //leser høyre knapp
		yourMessage.data[5] = adc_read(2); //Venstre slider
		yourMessage.data[6] = adc_read(3); //Høyre slider	
		can_send_message(&yourMessage);
		printf("Sent msg\n");
		_delay_ms(100);
	
		
		//uint8_t statreg = mcp2515_read_status();
		
		
		/*if(RECEIVE_BUFFER_INTERRUPT){ //Mulig å lage det som en funskjon i ettertid

			can_receive_message(&mottatt);		
			
			char mottatt_data_char0 = mottatt.data[0];
			char mottatt_data_char1 = mottatt.data[1];
			char mottatt_data_char2 = mottatt.data[2];
			char mottatt_data_char3 = mottatt.data[3];
			char mottatt_data_char4 = mottatt.data[4];
			char mottatt_data_char5 = mottatt.data[5];
			char mottatt_data_char6 = mottatt.data[6];
			char mottatt_data_char7 = mottatt.data[7];
						


			printf("ID: %i  LENGTH: %i   ALL DATA  %c    %c   %c    %c    %c    %c    %c    %c\n", mottatt.id , mottatt.length, mottatt_data_char0, mottatt_data_char1, mottatt_data_char2, mottatt_data_char3, mottatt_data_char4, mottatt_data_char5, mottatt_data_char6, mottatt_data_char7);
			RECEIVE_BUFFER_INTERRUPT = 0; //clearer interruptflagget
			
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
						
		}
		_delay_ms(500);*/
		if(joy_doesDirectionChange()){
			menu_save();
			oled_refresh();
		}
	}
	
	
	
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
}

