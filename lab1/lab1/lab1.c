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
#include "avr.h"

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
	enableExternalMemory(); 
	
	//Tester sram
	
	//SRAM_test();
	
	avr_init();

	volatile char *ext_ram = (char *) 0x1201; //For å aksessere external IC lager vi pointer til adresse
	volatile int8_t retreived_valueX;
	volatile int8_t retreived_valueY;
	printf("START ADC \n");
	
	volatile int8_t sliderValueLeft;
	volatile int8_t sliderValueRight;
	
	int leftButton = 0;
	int rightButton = 0;
	while(1){
		
		/*
		sliderValueLeft = adc_getLeftSlider();
		sliderValueRight = adc_getRightSlider();
		printf("Left///Right: %i /// %i \n", sliderValueLeft, sliderValueRight);
		*/
		
		
		//ext_ram[0] = 1;
		retreived_valueX = joy_getPercent(adc_getX()); 
		retreived_valueY = joy_getPercent(adc_getY());

		
		
		printf("Retrieved value, X/Y: %i /// %i \n", retreived_valueX, retreived_valueY);
		
		//printf("Retrieved value Y: %i \n", retreived_valueY);
		
		/*
		
		leftButton = PINB & (1<<PB0);
		rightButton = PINB & (1<<PB1);
		
		printf("Left button: %i Right button %i \n", leftButton, rightButton);
		*/
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
