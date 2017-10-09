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
#include "menu.h"
#include "spi.h"
#include "MCP2515.h"
//#include "fonts.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


int main(void)
{
	
	USART_Init(MYUBRR);
	extMem_init(); 
	avr_init();
	adc_init();
	oled_init();
	oled_clear_screen();
	menu_init();
	//spi_init();
	mcp2515_init();
	
	

	//LESER A
	while(1){
		uint8_t data;

		data = mcp2515_read(MCP_TXB0CTRL);

		printf("Returnert verdi %02X\n", mcp2515_read_status()); //printer verdien vi leser fra registeret i hex
		_delay_ms(1000);
	}
	
	
	
	//OPPDATERER OLED-SKJERMEN NÅR NOE JOYSTICKEN FLYTTES
	while(1){
		
		if(joy_doesDirectionChange()){
			menu_save();
			oled_refresh();
		}
	}
	
}



