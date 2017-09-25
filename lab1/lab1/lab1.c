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
#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "externalmemory.h"
#include "avr.h"
#include "oled.h"
#include "joy.h"
//#include "fonts.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


int main(void)
{
	
	USART_Init ( MYUBRR );
	extMem_init(); 
	avr_init();
	adc_init();
	oled_init();
	oled_clear_screen();
	//SRAM_test();
	
	
	//oled_goto_pos(0,0);

	

	oled_print("Chase a check, never     chase a bitch            Mask on, fuck it mask offMask on, fuck it mask off", 0,0);
	
	


}
