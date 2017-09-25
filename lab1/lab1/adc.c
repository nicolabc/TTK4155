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

volatile int adcInProgress = 0;

void adc_init(void){
	DDRE &= ~(1<<DDE0); //Hvis vi skal bruke interrupt pin-en i stedet for delay (blir en slags interrupt)
}

uint8_t adc_read(uint8_t ch){
	//ch = 0: joystick x. ch = 1: joystick y. ch = 2: slider left. ch = 3: slider right. 
	volatile char *ext_adc =(char*) 0x1400;
	*ext_adc = 0x4 + ch;
	//while(PINE & (1<<PINE0));
	_delay_us(60);
	return *ext_adc;	
}

/* DØD KODE - evt interrupt
int adc_getX(void){
	adcInProgress = 1;
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 4;
	while(adcInProgress);	
	//_delay_us(50);
	return (adc_start[0]);
}

int adc_getY(void){
	adcInProgress = 1;
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 5;
	while(adcInProgress);
	//_delay_us(50);
	return (adc_start[0]);
}

int adc_getLeftSlider(void){
	adcInProgress = 1;
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 6;
	//while(adcInProgress);
	_delay_us(50);
	return (((adc_start[0]*1.0)/255)*100);
}

int adc_getRightSlider(void){
	adcInProgress = 1;
	volatile char *adc_start = (char *) 0x1400;
	adc_start[0] = 7;
	//while(adcInProgress);
	_delay_us(50);
	return (((adc_start[0]*1.0)/255)*100);
}

ISR(INT2_vect){
	//Rising edge fordi tri state i adc når falling edge
	adcInProgress = 0; //Adc ferdig med konvertering når interrupt kalles
}
*/