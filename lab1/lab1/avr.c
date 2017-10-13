/*
 * avr.c
 *
 * Created: 18.09.2017 15:38:53
 *  Author: nicolabc
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "avr.h"

#define F_CPU 4915200
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

void avr_init(void){
	clear_bit(DDRB,PB0); //Setter PB0 til read
	clear_bit(DDRB,PB1); //Setter PB1 til read
	clear_bit(DDRB,PB2); //Setter PB2 til read
	set_bit(PORTB, PB2); //Setter pullup resistor for PB2
	
	
	
	cli(); //Clear interrupt enable
	clear_bit(EMCUCR, ISC2); // Falling edge interrupt trigger s.85 atmega162 datasheet
	set_bit(GICR,INT2); //Enable External Interrupt 2
	sei(); // Enable Global Interrupt
	
}