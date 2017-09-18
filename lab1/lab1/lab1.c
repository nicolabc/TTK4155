/*
 * lab1.c
 *
 * Created: 04.09.2017 08:51:27
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define F_CPU 4915200

#include "util/delay.h"
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "externalmemory.h"

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
	
	//set_bit(DDRA,PA1);	// setter PA1 til output
	//clear_bit(PORTA,PA1);	 // setter PA1 høy (så det blir 5V på utgangen til PA1)
	
	//set_bit(DDRE, PE1); //Lab 2, setter PE1 til output (Tror vi)
	//set_bit(PORTE, PE1); //Lab 2, setter PE1 til høy (Latch enable)
	
	/*clear_bit(DDRE, PE0); // Setter PE0 til lav for å lese fra pin (input, interrupt
	set_bit(PORTE, PE1); //
	*/
	
	//DDRA |= (1<<PA1); // setter PA1 til output
	//PORTA |= (1<<PA1); // setter PA1 høy (så det blir 5V på utgangen til PA1)
	

	
	//Tester sram
	enableExternalMemory(); //Trenger vi denne alltid på???
	//SRAM_test();
	
	
	
	/* Trengs ikke fordi vi kan aksessere med pointer
	//a11
	set_bit(DDRC,PC3); // DDRC |= (1 << PC3) Setter PC3 til output
	clear_bit(PORTC, PC3); // PORTC |= (1 << PC3) Setter PC3 høy
	
	//a10	
	set_bit(DDRC, PC2);
	set_bit(PORTC, PC2);
	
	//a9	
	set_bit(DDRC, PC1);
	set_bit(PORTC, PC1);
	
	//a8	
	set_bit(DDRC, PC0);
	set_bit(PORTC, PC0);
	*/
	
	volatile char *ext_ram = (char *) 0x1400; //For å aksessere external IC lager vi pointer til adresse
	volatile uint8_t retreived_valueX;
	volatile uint8_t retreived_valueY;
	printf("START ADC \n");
	while(1){
		//ext_ram[0] = 1;
		retreived_valueX = ADCreadX(); 
		retreived_valueY = ADCreadY();
		//ext_ram = 1; //For å faktisk gjøre noe med adressen må noe skrives dit
		//_delay_ms(1000);
		//ext_ram[1] = 3;
		
		
		printf("Retrieved value, X/Y: %i /// %i \n", retreived_valueX, retreived_valueY);
		
		//printf("Retrieved value Y: %i \n", retreived_valueY);
	}
	
	
	printf("START");
    while(1)
    {	
		
		uint8_t a = 1;
		ext_ram[0] = a;
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
