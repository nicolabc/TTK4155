/*
 * motor.c
 *
 * Created: 03.11.2017 15:00:31
 *  Author: danieta
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "dac.h"
#include "encoder.h"
#include "../../../lib/joy.h"

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

int16_t CALIBRATE_MAX;
int16_t CALIBRATE_MIN;
volatile double PREV_ERROR = 0;
volatile double SUM_ERROR = 0;
volatile int16_t PREV_ENCODERVALUE = 0;
volatile int FIRST_ENCODER_VALUE_READ = 0;

void motor_init(void){
	
	//Enable motor
	set_bit(DDRH,DDH4); //Output
	set_bit(PORTH, PH4); //Enable
	
	//Set direction
	set_bit(DDRH,DDH1); //Output
	clear_bit(PORTH,PH1); //Usikker på om dette er høyre eller venstre, kan evt ta clear_bit for motsatt vei
	
	motor_setVoltage(0); //For å ikke starte motor unødvendig
}

void motor_setVoltage(uint8_t myValue){
	dac_send(myValue); //Må fikse logikk på joystick evt slider
}

void motor_dirLeft(void){
	clear_bit(PORTH,PH1);
	
}

void motor_dirRight(void){
	set_bit(PORTH,PH1);
}

void motor_calibrate(void){
	motor_dirRight();
	motor_setVoltage(70);//start calibration by going to the right
	_delay_ms(3000);
	CALIBRATE_MAX = encoder_read();
	motor_dirLeft();
	_delay_ms(3000);
	CALIBRATE_MIN = encoder_read();
	motor_setVoltage(0);
	
	printf("Max: %i  Min: %i \n\n\n", CALIBRATE_MAX, CALIBRATE_MIN);
}


void motor_PIDspeed(int velRef, int16_t encoderValue, int Kp_in, int Ki_in, int Kd_in){
	if(FIRST_ENCODER_VALUE_READ == 0){ //For å ikke få feil første gang vi går inn (da er PREV_ENCODERVALUE satt default til 0. Dette blir da feil i utregningen)
		FIRST_ENCODER_VALUE_READ = 1;
		return;
	}
	
	
	int16_t dx = encoderValue - PREV_ENCODERVALUE;
	double dt = 0.05; //Timer interrupt seconds (every measurment is done with 0.05s interval
	double velocity = -((double)dx)/dt; //Positiv defineres til høyre
	velocity = velocity/1000; //For å ikke få altfor høye verdier
	PREV_ENCODERVALUE = encoderValue;
	
	double Kp;  //1.5;
	double Ki;  //0.5;
	double T =  0.05;  //0.05;
	double Kd; //0.07;
	
	//If all inputs are 0 then we use the standard parameters
	if(Kp_in+Ki_in+Kd_in == 0){
		Kp = 1.2;  //1.5;
		Ki = 0.5;  //0.5;
		Kd = 0.05; //0.07;
	}
	
	else{
		Kp = ((double)Kp_in) / 10;  
		Ki = ((double)Ki_in) / 10;  
		Kd = ((double)Kd_in) / 10;
	}
	
	
	velRef = velRef;
	double error = (velRef - velocity); 
	int errorThreshold = 5;
	double AbsError = abs(error); //Siden vi kun er interessert i feil, vi tar for oss retning i if(error > 10) .. else if (error <10)...
	if(error > errorThreshold){
		SUM_ERROR = SUM_ERROR + error;
	}else{
		SUM_ERROR = 0;
	}
	
	
	
	
	
	//-------Diskret PD regulator
	int u = Kp*AbsError+ T*Ki*SUM_ERROR  + Kd/T*(error-PREV_ERROR); //
	
	//printf("Velocity: %i error: %i    u:  %i \n", (int)velocity,(int)error, u);
	//Velg retning basert på hvilken vei joystick peker
	if(error > errorThreshold){
		motor_dirRight();
		}else if(error < -errorThreshold){
		motor_dirLeft();
	}else{
		u = 0;
	}
	if(u<250 && u >= 0){
		motor_setVoltage(u); //input to motor
		}else{
		motor_setVoltage(150); //Max
	}
	
	PREV_ERROR = error;
}