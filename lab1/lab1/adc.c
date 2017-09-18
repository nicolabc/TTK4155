/*
 * adc.c
 *
 * Created: 18.09.2017 09:43:45
 *  Author: nicolabc
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 4915200

#include "util/delay.h"
#include "adc.h"
#include "joy.h"

/*
void adc_init(void){
	
}
*/

int adc_getX(void){
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 4;
	_delay_us(50);
	
	return (adc_start[0]);
}

int adc_getY(void){
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 5;
	_delay_us(50);
	return (adc_start[0]);
}

int adc_getLeftSlider(void){
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 6;
	_delay_us(50);
	return (((adc_start[0]*1.0)/255)*100);
}

int adc_getRightSlider(void){
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 7;
	_delay_us(50);
	return (((adc_start[0]*1.0)/255)*100);
}
