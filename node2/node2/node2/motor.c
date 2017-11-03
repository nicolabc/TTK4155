/*
 * motor.c
 *
 * Created: 03.11.2017 15:00:31
 *  Author: danieta
 */ 

#include <avr/io.h>
#include "motor.h"
#include "dac.h"

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

void motor_init(void){
	
	//Enable motor
	set_bit(DDRH,DDH4); //Output
	set_bit(PORTH, PH4); //Enable
	
	//Set direction
	set_bit(DDRH,DDH1); //Output
	clear_bit(PORTH,PH1); //Usikker på om dette er høyre eller venstre, kan evt ta clear_bit for motsatt vei
}

void motor_setVoltage(uint8_t myValue){
	dac_send(myValue); //Må fikse logikk på joystick evt slider
}

void motor_dirLeft(void){
	
	set_bit(PORTH,PH1);
}

void motor_dirRight(void){
	clear_bit(PORTH,PH1);
}
