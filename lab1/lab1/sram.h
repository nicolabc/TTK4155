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
void sram_write_data(uint8_t data);
void sram_goto_page(uint8_t newPage);
void sram_goto_column(uint8_t newColumn);
void sram_save_char(char myChar);
void sram_save_string(char* myString, uint8_t page, uint8_t col);

#endif /* SRAM_H_ */