/*
 * oled.h
 *
 * Created: 22.09.2017 10:32:49
 *  Author: danieta
 */ 


#ifndef OLED_H_
#define OLED_H_

/*Writes a string to the oled*/
void write_c(uint8_t command);

/*Refreshes the screen from SRAM*/
void oled_refresh(void);

/*Initializes relevant pins to input/output*/
void oled_init();

/*Goes to a specified page using page addressing mode*/
void oled_goto_page(uint8_t page);

/*Goes to a specified column using page addressing mode*/
void oled_goto_column(uint8_t column);

/*Goes to a specified location on the screen using page addressing mode*/
void oled_goto_pos(uint8_t page, uint8_t col);

/*Clears the screen*/
void oled_clear_screen(void);



#endif /* OLED_H_ */