/*
 * internalADC.c
 *
 * Created: 27.10.2017 16:39:39
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include <stdint.h>
#include "internalADC.h"

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit )(reg &= ~(1 << bit))
#define test_bit( reg, bit )(reg & (1 << bit))


void internalADC_init(void){
	
	clear_bit(DDRF,PF0); //Set A0 to input
	set_bit(ADCSRA,ADEN); //ADC enable
	
	//clear_bit(ADCSRB, MUX5);//Setter ADC til konvertering av ADCO
	clear_bit(ADMUX,MUX4);// ----||----
	clear_bit(ADMUX,MUX3);// ----||----
	clear_bit(ADMUX,MUX2);// ----||----
	clear_bit(ADMUX,MUX1);// ----||----
	clear_bit(ADMUX,MUX0);// ----||----
	
	clear_bit(ADMUX,REFS1);// Voltage reference to AVcc
	set_bit(ADMUX,REFS0); // ----||----
	
	clear_bit(ADCSRA,5);  //Turn off free running mode
	
	set_bit(ADCSRA,ADIE); //Enable interrupt
	
	//setter prescaler til 4, funker ikke med 2
	clear_bit(ADCSRA, ADPS0);
	set_bit(ADCSRA, ADPS1);
	clear_bit(ADCSRA, ADPS2);
	
	internalADC_startConversion();//Starter første konvertering
}

uint16_t internalADC_getResult(void){
	
	uint16_t resultL = ADCL;
	uint16_t resultH = ADCH; //Leftshift 8 ganger
	resultH = (resultH << 8);
	uint16_t result = resultH + resultL;
	
	return result;
	
}

void internalADC_startConversion(void){
	set_bit(ADCSRA,ADSC); //Start conversion
}

