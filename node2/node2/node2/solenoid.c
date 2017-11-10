/*
 * solnoid.c
 *
 * Created: 06.11.2017 13:12:08
 *  Author: nicolabc
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/delay.h>
#include "solenoid.h"

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


void solenoid_init(void){
	//Setter pin A1 til output og høy
	set_bit(DDRF, PF1);
	clear_bit(PORTF, PF1); //A1 høy
}

void solenoid_shoot(void){
	// Seter en puls på pin A1
	set_bit(PORTF, PF1);
	_delay_ms(1000);
	clear_bit(PORTF, PF1);
}