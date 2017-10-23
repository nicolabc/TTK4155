/*
 * timer.c
 *
 * Created: 23.10.2017 08:58:45
 *  Author: nicolabc
 */ 


#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "timer.h"

#define F_CPU 16000000
#define FOSC 16000000// Clock Speed

//Fra node 1
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

void timer_init(){
	
	//Fast PWM set up. s.145 in ATmega2560 datasheet
	clear_bit(TCCR1A,WGM10);
	set_bit(TCCR1A,WGM11);
	set_bit(TCCR1B,WGM12);
	set_bit(TCCR1B,WGM13);
	
	//Setter til non-inverted PWM, s. 155
	set_bit(TCCR1A,COM1A1);
	clear_bit(TCCR1A,COM1A0);
	
	//Setter prescaler til 256 s.156-157
	set_bit(TCCR1B,CS12);
	clear_bit(TCCR1B,CS11);
	clear_bit(TCCR1B,CS10);
	
	//Setter OC1A til output (Skal være PB5 i DDRA register?)
	set_bit(DDRB, PB5);
	
	
	
	//Setter maksverdi for 20ms periode
	uint16_t prescaler = 256;
	uint16_t F_OC1A = 50; // 1/(20*10^-3)
	uint16_t top = (F_CPU)/(prescaler*F_OC1A)-1; // = 1249 
	ICR1 = top;
	
	//cli();
	
	//Enable timer overflow interrupt (Altså når en full PWM syklus har gått (20ms))
	//set_bit(TIMSK1,TOIE1);
	
	//sei();
}

int timer_dutyCycleUpdate(int percent){
	
	if (percent <  0 || percent > 100)
	{
		return -1;
	}
	uint16_t top = (percent*(131-56))/100+56;
	OCR1A = top;
	
	return 1;
	
}
