/*
 * oled.c
 *
 * Created: 22.09.2017 10:33:00
 *  Author: danieta
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "oled.h"
#include "fonts.h"


void write_c(uint8_t command){
	volatile char *ext_oled_c = (char *) (0x1000);
	ext_oled_c[0] = command;
}

void oled_write_data(uint8_t data){
	volatile char *ext_oled_d = (char *) (0x1200);
	ext_oled_d[0] = data;
}

void oled_goto_page(uint8_t page){
	write_c(0xb0+page);
}

void oled_goto_column(uint8_t column){ //From 0x00 to 0x7E
	
	uint8_t lastFourBits = column & (0b00001111);
	uint8_t firstFourBits = (column & (0b11110000)) >> 4;
	
	write_c(0x00+lastFourBits); //Siden de siste fire bitsene i column-bitsene v�re representerer siste hex-verdien til column (f.eks. om column er 7E, s� blir lastFourBits E).
	write_c(0x10+firstFourBits);
	//Slik pekerne til oled er definert, m� vi dele opp column i 7 og E (om column er 7E) og bruke write_c for � skrive riktig kommando ut fra hva lower column og higher column er i tabell 8.1 - command table
}

void oled_goto_pos(uint8_t page, uint8_t col){
	oled_goto_page(page);
	oled_goto_column(col);
}


void oled_clear_screen(void){ //Clears the entire screen
	int i = 0;
	int j = 0;
	for(i = 0; i <8; i++){
		oled_goto_page(i);
		for(j = 0; j < 128; j++){
			oled_write_data(0b00000000); //To clear a single column
		} 
	}
}
void oled_print_char(char myChar){
	int asciValue = myChar;
	
	int number = asciValue -32;

	int i = 0;
	for(i=0; i<5; i++){
		oled_write_data(pgm_read_byte(&font5[number][i])); //siden vi m� aksessere programminnet, m� vi bruke pgm_read_byte
	}
}

void oled_print(char* myString, uint8_t page, uint8_t col){ //har med page og col her s� jeg slipper � bruke oled_goto_pos f�r jeg vil printe noe
	oled_goto_pos(page,col);
	int i = 0;
	int xPosition = col;
	printf("%i", strlen((myString)));
	int j = 0;
	for (i = 0; i < strlen(myString); i++){
		if(5*j + xPosition > 123){ //OM VI ENDRER FONT SIZE FRA FONT5, M� VI ENDRE 5*i OGS�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			oled_goto_pos(++page,0);
			xPosition = 0;
			j = 0;
		}
		j++;
		oled_print_char(myString[i]);
	}
}

void oled_init(){
	
	write_c(0xae);        //  display  off
	write_c(0xa1);        //segment  remap
	write_c(0xda);        //common  pads  hardware:  alternative
	write_c(0x12);
	write_c(0xc8);        //common output scan direction:com63~com0
	write_c(0xa8);        //multiplex  ration  mode:63
	write_c(0x3f);
	write_c(0xd5);        //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81);        //contrast  control
	write_c(0x50);
	write_c(0xd9);        //set  pre-charge  period
	write_c(0x21);
	write_c(0x20);        //Set  Memory  Addressing  Mode -- Page addressing mode
	write_c(0x02);
	write_c(0xdb);        //VCOM  deselect  level  mode
	write_c(0x30);
	write_c(0xad);        //master  configuration
	write_c(0x00);
	write_c(0xa4);        //out  follows  RAM  content
	write_c(0xa6);        //set  normal  display
	write_c(0xaf);        //  display  on
}