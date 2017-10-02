/*
 * oled.h
 *
 * Created: 22.09.2017 10:32:49
 *  Author: danieta
 */ 


#ifndef OLED_H_
#define OLED_H_

void write_c(uint8_t command);
//void oled_write_data(uint8_t data);
void oled_write_data(uint8_t data, uint8_t page, uint8_t col);
void oled_refresh(void);
void oled_init();
void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);
void oled_goto_pos(uint8_t page, uint8_t col);
void oled_clear_screen(void);
void oled_print_char(char myChar);
void oled_print(char* myString, uint8_t page, uint8_t col);

//void oled_print(char*);

/*void oled_reset();
void oled_home();

void oled_clear_line(line);

void oled_set_brightness(lvl);
*/


#endif /* OLED_H_ */