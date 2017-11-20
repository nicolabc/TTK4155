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
#include "../../lib/joy.h"


volatile int adcInProgress = 0;

void adc_init(void){
	DDRE &= ~(1<<DDE0); 
}

uint8_t adc_read(uint8_t ch){ //ch = 0: joystick x. ch = 1: joystick y. ch = 2: slider left. ch = 3: slider right. 
	
	
	volatile char *ext_adc =(char*) 0x1400;
	*ext_adc = 0x4 + ch;
	

	
	
	_delay_us(50);
	return *ext_adc;	
}
