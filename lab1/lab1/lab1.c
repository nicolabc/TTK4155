/*
 * lab1.c
 *
 * Created: 04.09.2017 08:51:27
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include <stdio.h>

#define F_CPU 4915200

#include "util/delay.h"
#include "uart.h"
#include "sram.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
//#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
//#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )


int main(void)
{
	USART_Init ( MYUBRR );
	set_bit(DDRA,PA1);	// setter PA1 til output
	clear_bit(PORTA,PA1);	 // setter PA1 høy (så det blir 5V på utgangen til PA1)
	
	set_bit(DDRE, PE1); //Lab 2, setter PE1 til output (Tror vi)
	set_bit(PORTE, PE1); //Lab 2, setter PE1 til høy (Latch enable)
	
	
	//DDRA |= (1<<PA1); // setter PA1 til output
	//PORTA |= (1<<PA1); // setter PA1 høy (så det blir 5V på utgangen til PA1)
	
	
    while(1)
    {
		/*PORTA &= ~(1<<PA1);
        //TODO:: Please write your application code 
		_delay_ms(3000);
		PORTA |= (1<<PA1);*/
		
		USART_Transmit('a');
		//_delay_ms(1000);
		
		printf(" 3boi\n");
		_delay_ms(1000);
			
			
		
		/*char c = USART_Receive();
		USART_Transmit(c);*/
	}
}
