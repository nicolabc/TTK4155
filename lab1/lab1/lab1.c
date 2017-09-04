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

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


int main(void)
{
	USART_Init ( MYUBRR );
	DDRA |= (1<<PA1); // setter PA1 til output
	PORTA |= (1<<PA1); // setter PA1 høy (så det blir 5V på utgangen til PA1)
	
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
