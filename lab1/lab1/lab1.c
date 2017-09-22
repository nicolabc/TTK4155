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
#include "oled.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


int main(void)
{
	USART_Init ( MYUBRR );
	enableExternalMemory(); 
	
	//Tester sram
	
	//SRAM_test();
	
	avr_init();
	

	volatile char *ext_ram = (char *) 0x1201; //For å aksessere external IC lager vi pointer til adresse
	adc_init();
	while(1){
		
		/*
		sliderValueLeft = adc_getLeftSlider();
		sliderValueRight = adc_getRightSlider();
		
		leftButton = PINB & (1<<PB0);
		rightButton = (PINB & (1<<PB1)); //Blir 2, men kan bruke if
		
		printf("Left///Right: %i /// %i /// Left button: %i Right button %i \n", sliderValueLeft, sliderValueRight, leftButton, rightButton );
		
		*/
		
		//ext_ram[0] = 1;

		//

		
		
		//printf("Retrieved value, X/Y: %i /// %i \n", retreived_valueX, retreived_valueY);
		//printf("Retrieved value, X: %i \n", retreived_valueX);
		//printf("Retrieved value, Y: %i \n", retreived_valueY);

		//printf("                                                     Retrieved value Y: %i \n", retreived_valueY);
		
		
		
		printf("Left: %i //////  right: %i ////// L3: %i \n", joy_readButton(0), joy_readButton(1), joy_readButton(2));
		
		//printf("Left button: %i Right button %i \n", leftButton, rightButton);
		
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
