/*
 * lab1.c
 *
 * Created: 04.09.2017 08:51:27
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include "util/delay.h"


int main(void)
{
	DDRA |= (1<<PA1); // setter PA1 til output
	PORTA |= (1<<PA1); // setter PA1 høy (så det blir 5V på utgangen til PA1)
	
    while(1)
    {
		PORTA &= ~(1<<PA1);
        //TODO:: Please write your application code 
		_delay_ms(3000);
		PORTA |= (1<<PA1);
		_delay_ms(3000);
	}
}