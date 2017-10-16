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

//Fra node 1
#define F_CPU 16000000
#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


volatile int RECEIVE_BUFFER_INTERRUPT = 0;

int main(void)
{
	USART_Init(MYUBRR);
	can_init();
	
	
	can_msg melding;
	melding.id = 5;
	melding.length = 8;
	melding.data[0] = (uint8_t)('S');
	melding.data[1] = (uint8_t)('K');
	melding.data[2] = (uint8_t)('Y');
	melding.data[3] = (uint8_t)('Y');
	melding.data[4] = (uint8_t)('Y');
	melding.data[5] = (uint8_t)('Y');
	melding.data[6] = (uint8_t)('A');
	melding.data[7] = (uint8_t)('A');
	
	printf("NY\n");
    while(1)
    {
		
		can_send_message(&melding);
		can_msg mottatt;
		
		
		//sjekker om receive bufre inneholder noe. se s. 69 i mcp2515
		uint8_t statusReg = mcp2515_read_status();
		
		if (test_bit(statusReg, 0))
		{
			mottatt = can_receive_message();
			
			char mottatt_data_char0 = mottatt.data[0];
			char mottatt_data_char1 = mottatt.data[1];
			char mottatt_data_char2 = mottatt.data[2];
			char mottatt_data_char3 = mottatt.data[3];
			char mottatt_data_char4 = mottatt.data[4];
			char mottatt_data_char5 = mottatt.data[5];
			char mottatt_data_char6 = mottatt.data[6];
			char mottatt_data_char7 = mottatt.data[7];
				
			printf("ID: %i  LENGTH: %i   ALL DATA  %c    %c   %c    %c    %c    %c    %c    %c\n", mottatt.id , mottatt.length, mottatt_data_char0, mottatt_data_char1, mottatt_data_char2, mottatt_data_char3, mottatt_data_char4, mottatt_data_char5, mottatt_data_char6, mottatt_data_char7);
		
		}
		/*
		if (test_bit(statusReg, 1))
		{
			printf("Mottatt melding på receive buffer 1");
		}*/
		
	
		
    }
	
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
}