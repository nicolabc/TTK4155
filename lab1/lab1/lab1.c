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
#include "../../lib/uart.h"
#include "sram.h"
#include "adc.h"
#include "externalmemory.h"
#include "avr.h"
#include "oled.h"
#include "menu.h"
#include "../../lib/spi.h"
#include "../../lib/MCP2515.h"
#include "../../lib/can.h"
#include "../../lib/joy.h"
#include "multiboardInfo.h"
//#include "fonts.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

volatile int RECEIVE_BUFFER_INTERRUPT = 0;

//Kan legge til flere states slik at spillet utvikler seg. F.eks hver vanskelighestgrad
/*
enum gamestate {
	MENU = 0,
	PLAYING = 1,
};*/

int GAMESTATUS = 0;

int main(void)
{
	
	USART_Init(MYUBRR);
	extMem_init(); 
	
	
	avr_init();
	adc_init();
	oled_init();
	oled_clear_screen();
	menu_init();
	can_init();
	
	can_msg yourMessage;
	GAMESTATUS = MENU;
	uint8_t gameOverValue = 0; //verdien vi f�r fra can-meldinga fra node 2 om vi har mista ballen i bakken
	
	while(1)
	{
		multiboardInfo_update(&yourMessage);
		can_send_message(&yourMessage);
		
		//Leser statusregisteret og sjekker om recieve-bufferet har f�tt inn noe
		volatile uint8_t statusReg = mcp2515_read_status();
		
		if(test_bit(statusReg, 0)){
			can_msg mottatt;
			can_receive_message(&mottatt);
			gameOverValue = mottatt.data[0];
			
			//printf("ID: %i   Length: %i   GameOver: %i \n", mottatt.id, mottatt.length, gameOverValue);	
		}
		
		
		
		switch(GAMESTATUS){
			case MENU:
				if(joy_doesDirectionChange()){
					
					menu_save();
					oled_refresh();
				}
				
				break;
			case PLAYING_EASY:
				oled_clear_screen(); //Litt dumt at alt refresher hver gang i main n� da :)
				menu_printGameScreen();
				oled_refresh();
				if(gameOverValue == 1){
					GAMESTATUS = GAMEOVER;
				}
				break;
				
				
			case PLAYING_NORMAL:
				oled_clear_screen();
				menu_printGameScreen();
				oled_refresh();
				if(gameOverValue == 1){
					GAMESTATUS = GAMEOVER;
				}
				break;
				
				
			case PLAYING_HARD:
				oled_clear_screen();
				menu_printGameScreen();
				oled_refresh();
				if(gameOverValue == 1){
					GAMESTATUS = GAMEOVER;
				}
				break;
				
				
			case GAMEOVER:
				oled_clear_screen();
				sram_save_string("GAME OVER",3,30);
				printf("GAME OVER MANN");
				oled_refresh();
			default:
			break;
		}
		
	}
	
	
	
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
}

