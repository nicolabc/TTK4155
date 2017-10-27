/*
 * node2.c
 *
 * Created: 16.10.2017 13:20:09
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "../../../lib/uart.h"
#include "../../../lib/spi.h"
#include "../../../lib/MCP2515.h"
#include "../../../lib/can.h"
#include "timer.h"
#include "servo.h"
#include "internalADC.h"


#define F_CPU 16000000
#define FOSC 16000000// Clock Speed

//Fra node 1
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


volatile int RECEIVE_BUFFER_INTERRUPT = 0;
volatile int TIMER_OVERFLOW_INTERRUPT = 0;

int main(void)
{
	
	USART_Init(MYUBRR);
	can_init();
	timer_init();
	internalADC_init();
	
	can_msg melding;
	melding.id = 5;
	melding.length = 8;
	melding.data[0] = (uint8_t)('1');
	melding.data[1] = (uint8_t)('2');
	melding.data[2] = (uint8_t)('3');
	melding.data[3] = (uint8_t)('4');
	melding.data[4] = (uint8_t)('5');
	melding.data[5] = (uint8_t)('6');
	melding.data[6] = (uint8_t)('7');
	melding.data[7] = (uint8_t)('8');
	
	int mottatt_data_char0 = 50; //startposisjon
    while(1)
    {
		
		//	can_send_message(&melding);
		can_msg mottatt;
		
		
		//sjekker om receive bufre inneholder noe. se s. 69 i mcp2515
		volatile uint8_t statusReg = mcp2515_read_status();
		
		//printf("statusReg: %x \n", statusReg);
		
		if(test_bit(statusReg, 0)){ //Mulig å lage det som en funskjon i ettertid

			can_receive_message(&mottatt);		
			
			mottatt_data_char0 = mottatt.data[0]; //X-akse
			int mottatt_data_char1 = mottatt.data[1];
			int mottatt_data_char2 = mottatt.data[2];
			int mottatt_data_char3 = mottatt.data[3];
			int mottatt_data_char4 = mottatt.data[4];
			int mottatt_data_char5 = mottatt.data[5];
			int mottatt_data_char6 = mottatt.data[6];
						
			

			printf("ID: %i  LENGTH: %i   ALL DATA  %i    %i   %i    %i    %i    %i    %i   \n", mottatt.id , mottatt.length, mottatt_data_char0, mottatt_data_char1, mottatt_data_char2, mottatt_data_char3, mottatt_data_char4, mottatt_data_char5, mottatt_data_char6);
			RECEIVE_BUFFER_INTERRUPT = 0; //clearer interruptflagget
			
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
			
			
			
					
		}
		servo_positionUpdate(mottatt_data_char0);

		
    }
	
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
}
/*
ISR(TIMER1_OVF_vect){
	TIMER_OVERFLOW_INTERRUPT +=1;
}*/