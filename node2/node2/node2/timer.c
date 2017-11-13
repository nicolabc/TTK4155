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
	
	
	/*
	http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
	*/
	//Enable timer compare interrupt for waking up program
	
	
	
	timer_timedInterrupt(); //Enable timed interrupt (FOR PID)
	
	
	
}
void timer_timedInterrupt(void){
	// ----- Timed interrupt for whole Node 2 -----
	
	
	double frequency = (F_CPU/256);
	double T = 1/frequency; //Period of prescaler
	double targetTime = 0.05;
	
	//FOrmel under funker ikke pga overflow, setter verdi manuelt
	//timercount = targetTime/(1.6*10^(-5));
	//int timerCount = targetTime/(T)-1; //6249. -1 fordi det tar en klokkesykel å resette klokken.
	
	int timerCount = 625*5; //Nå oppdateres det hvert 0.05 sekund
	//Set compare match register to desired timer count
	OCR3A = timerCount;
	
	
	
	TCCR3A = 0;     // set entire TCCR3A register to 0
	TCCR3B = 0;     // same for TCCR3B
	
	//Enable CTC
	set_bit(TCCR3B,WGM32);
	
	//Prescaler 256
	set_bit(TCCR3B,CS12);
	clear_bit(TCCR3B,CS11);
	clear_bit(TCCR3B,CS10);
	
	//Enable timer compare interrupt
	set_bit(TIMSK3,OCIE3A);
}
int timer_dutyCycleUpdate(int percent){
	
	if (percent <  0 || percent > 100)
	{
		return -1;
	}
	uint16_t offset = 0; //Offset må endres utifra bord. 17 er for arbeidsplass 10. NÅ ER DET BORD 3! med 0 offset
	uint16_t top = (percent*(131-56))/100+56 -offset;
	OCR1A = top;
	
	return 1;
	
}
