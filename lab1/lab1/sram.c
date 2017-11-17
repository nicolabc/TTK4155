/*
 * sram.c
 *
 * Created: 11.09.2017 09:28:55
 *  Author: nicolabc
 */ 

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "sram.h"
#include "../../lib/uart.h"
#include "fonts.h"
#include "oled.h"
#include "../../lib/MCP2515.h"
#include "../../lib/joy.h"

volatile uint8_t PAGE;
volatile uint8_t COL;

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
		
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n %4d errors in retrieval phase \n\n", write_errors, retrieval_errors);
}

void sram_write_data(uint8_t data){
    volatile char *ext_sram = (char *) (0x1800);
    ext_sram[128*PAGE+COL] = data;
}

void sram_goto_page(uint8_t newPage){
	PAGE = newPage;
}

void sram_goto_column(uint8_t newColumn){
	COL = newColumn;
}

void sram_save_char(char myChar){
	int asciValue = myChar;
	int number = asciValue -32;
	int i = 0;
	
	//IF-SETNINGER FOR DE NYE CHARACTERENE VI LAGER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for(i=0; i<5; i++){
		if(asciValue == 198){ //Æ
			sram_write_data(pgm_read_byte(&font5[95][i]));
			COL++;
		}
		else if(asciValue == 230){ //æ
			
			sram_write_data(pgm_read_byte(&font5[96][i]));
			COL++;
		}
		else if(asciValue == 168){
			//siden ¨ ikke er i fonts, bruker vi denne til å lage smiley
			sram_write_data(pgm_read_byte(&font5[97][i]));
			COL++;
		}
		else{
			sram_write_data(pgm_read_byte(&font5[number][i]));
			COL++;
		}
	}
}

void sram_save_string(char* myString, uint8_t page, uint8_t col){
	PAGE = page;
	COL = col;
	int xPosition = COL;
	int characterNr = 0;
	int colCounter = 0;

	//kode for å gå til neste linje (page) om col er nærme slutten av pagen, horisontalt sett
	for (characterNr = 0; characterNr < strlen(myString); characterNr++){
		if((5*colCounter + xPosition) > 123){ //OM VI ENDRER FONT SIZE FRA FONT5, MÅ VI ENDRE 5*colCounter OGSÅ, og vi må endre oled_print_char sine for-løkke-lengder!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    		++PAGE;
			COL = 0;
			xPosition = 0;
    		colCounter = 0;
		}
		colCounter++;
		sram_save_char(myString[characterNr]);
	}

}

void sram_gameScreen(void){
	switch(GAMESTATUS){
		case PLAYING_EASY:
		sram_save_string("EASY", 0,30);
		sram_save_string("TOUCH LEFT BUTTON TO QUIT", 5,20);
		
		break;
		case PLAYING_NORMAL:
		sram_save_string("NORMAL",0,30);
		sram_save_string("TOUCH LEFT BUTTON TO QUIT", 5,20);
		break;
		case PLAYING_HARD:
		sram_save_string("HARD",0,30);
		sram_save_string("TOUCH LEFT BUTTON TO QUIT", 5,20);
		break;
		
		case GAMEOVER:
		sram_save_string("GAME OVER",3,40);
		sram_save_string("TOUCH LEFT BUTTON TO QUIT", 5,20);
		break;
		
		case PLAYING_CUSTOM:
		sram_save_string("CUSTOM",0,30);
		sram_save_string("TOUCH LEFT BUTTON TO QUIT", 5,20);
		break;
	}

}