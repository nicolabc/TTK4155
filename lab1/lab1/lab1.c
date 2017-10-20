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
	
	can_msg melding;
	melding.id = 6;
	melding.length = 8;
	melding.data[0] = (uint8_t)('S');
	melding.data[1] = (uint8_t)('K');
	melding.data[2] = (uint8_t)('K');
	melding.data[3] = (uint8_t)('L');
	melding.data[4] = (uint8_t)('M');
	melding.data[5] = (uint8_t)('N');
	melding.data[6] = (uint8_t)('O');
	melding.data[7] = (uint8_t)('P');
	

	while(1)
	{
		
		
		can_send_message(&melding);
		uint8_t statreg = mcp2515_read_status();
	//	printf("StatReg: %x \n",statreg);
		can_msg mottatt;
		
		
		printf("here\n");
		//sjekker om receive bufre inneholder noe. se s. 69 i mcp2515
		uint8_t statusReg = mcp2515_read_status();
		
	//	printf("statusReg: %x \n", statusReg);
		
		if(RECEIVE_BUFFER_INTERRUPT){ //Mulig å lage det som en funskjon i ettertid
/*
			can_receive_message(&mottatt);		
			
			char mottatt_data_char0 = mottatt.data[0];
			char mottatt_data_char1 = mottatt.data[1];
			char mottatt_data_char2 = mottatt.data[2];
			char mottatt_data_char3 = mottatt.data[3];
			char mottatt_data_char4 = mottatt.data[4];
			char mottatt_data_char5 = mottatt.data[5];
			char mottatt_data_char6 = mottatt.data[6];
			char mottatt_data_char7 = mottatt.data[7];
						*/


			char mottatt_data_char0 = buffer[0];
			char mottatt_data_char1 = buffer[1];
			char mottatt_data_char2 = buffer[2];
			char mottatt_data_char3 = buffer[3];
			char mottatt_data_char4 = buffer[4];
			char mottatt_data_char5 = buffer[5];
			char mottatt_data_char6 = buffer[6];
			char mottatt_data_char7 = buffer[7];
			printf("ID: %i  LENGTH: %i   ALL DATA  %c    %c   %c    %c    %c    %c    %c    %c\n", mottatt.id , mottatt.length, mottatt_data_char0, mottatt_data_char1, mottatt_data_char2, mottatt_data_char3, mottatt_data_char4, mottatt_data_char5, mottatt_data_char6, mottatt_data_char7);
			RECEIVE_BUFFER_INTERRUPT = 0; //clearer interruptflagget
			
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
						
		}
		_delay_ms(500);
		if(joy_doesDirectionChange()){
			menu_save();
			oled_refresh();
		}
	}
	
	
	
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
	can_msg mottatt;
	can_receive_message(&mottatt);
	memcpy(buffer + lengthOfBuffer, mottatt.data, mottatt.length);
	lengthOfBuffer += mottatt.length;
}

