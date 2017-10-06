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
	spi_MasterInit();
	
	//oled_refresh(); //Usikkert om denne trengs eller ikke fordi forrige initialisering huskes i SRAM
	
	printf("Tâz1\n");
	spi_MasterTransmit(MCP_WRITE, 1); //Velger instruction
	
	spi_MasterTransmit(MCP_TXB0SIDH , 1); //Velger adresse 0b00110001
	
	spi_MasterTransmit(0xAA, 0); //Velger data som sendes
	
	while(1){
			spi_MasterTransmit(MCP_READ, 1);
			spi_MasterTransmit(MCP_TXB0SIDH, 1);
			uint8_t returnedData = spi_MasterTransmit(0x00, 0);
			
			printf("Returnert verdi %02X\n", returnedData);
			_delay_ms(1000);
	}
	printf("Tâz");
	while(1){
		
		if(joy_doesDirectionChange()){
			menu_save();
			oled_refresh();
		}
		//printf("Y:    %i    %i \n",joy_getDirectionY(adc_read(1)),adc_read(1));

	}
	
	


}

void writeToRegister(uint8_t register, uint8_t data){
	spi_MasterTransmit(MCP_WRITE, 1); //Velger instruction
	
	spi_MasterTransmit(register , 1); //Velger adresse 0b00110001
	
	spi_MasterTransmit(data, 0); //Velger data som sendes
}

