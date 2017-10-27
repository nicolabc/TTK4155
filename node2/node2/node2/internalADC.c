/*
 * internalADC.c
 *
 * Created: 27.10.2017 16:39:39
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include "internalADC.h"

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


void internalADC_init(void){
	set_bit(ADCSRA,ADEN); //ADC enable
	
	clear_bit(ADCSRB, MUX5)//Setter ADC til konvertering av ADCO
	clear_bit(ADMUX,MUX4);// ----||----
	clear_bit(ADMUX,MUX3);// ----||----
	clear_bit(ADMUX,MUX2);// ----||----
	clear_bit(ADMUX,MUX1);// ----||----
	
	clear_bit(ADMUX,REFS1);// Voltage reference to AVcc
	set_bit(ADMUX,REFS0); // ----||----
	
	set_bit(ADCSRA,ADSC); //Start conversion
}

uint16_t internalADC_getResult(void){
	
	uint16_t resultL = ADCL;
	uint16_t resultH = (ADCH<<8); //Leftshift 8 ganger
	return resultL+resultH;
}