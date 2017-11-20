/*
 * sram.h
 *
 * Created: 11.09.2017 09:28:41
 *  Author: nicolabc
 */ 


#ifndef SRAM_H_
#define SRAM_H_

extern volatile uint8_t PAGE;
extern volatile uint8_t COL;

void SRAM_test(void);

/*Writes the data (in bits) to the SRAM*/
void sram_write_data(uint8_t data);

/*Changes the global variable PAGE to newPage*/
void sram_goto_page(uint8_t newPage);

/*Changes the global variable COL to newColumn*/
void sram_goto_column(uint8_t newColumn);

/*Saves the character in SRAM*/
void sram_save_char(char myChar);

/* Saves the string in SRAM */
void sram_save_string(char* myString, uint8_t page, uint8_t col);

/*Saves the gamescreen to SRAM*/
void sram_gameScreen(void);

#endif /* SRAM_H_ */