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

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))

int16_t CALIBRATE_MAX;
int16_t CALIBRATE_MIN;
volatile double PREV_ERROR = 0;
volatile double SUM_ERROR = 0;

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

void motor_PID(int posRef, int16_t encoderValue){	
	double scaledRef = (((double)posRef)/255)*100;
	
	int scalingfactor = abs(CALIBRATE_MAX-CALIBRATE_MIN);//Regner ut total avstand fra høyre til venstre på box
	double scaledEncoder = (((double)encoderValue)/scalingfactor)*100;
	
	
	//Altså mot høyre så går verdien nedover, men fra vår referanse skal verdien øke. Dermed må verdien øke som gjøres ved å gange med -1
	//KAN VÆRE HER NOE PROBLEMER OPPSTÅR --- ARBEIDSPLASS 6 BOX
	if (CALIBRATE_MAX<CALIBRATE_MIN){
		scaledEncoder = scaledEncoder*(-1);
	}
	
	//int16_t scaleToBoxRef = 35*posRef; //Encoderverdiene tilsvarer ca 35 ganger mer. Altså 255 på multifunctionboard tilsvarer 8750 i encoderverdi (pos til motor)
	double  error = scaledRef - scaledEncoder;
	//printf("Ref: %i  EncoderValue:  %i   Error: %i \n", scaleToBoxRef, encoderValue, error);
	
	double Kp = 0.2; //0.3
	double Ki = 0.5; //0.5
	double T = 0.05; //Sample time
	uint8_t Kd = 1;
	
	
	uint8_t u = 2.55*(Kp*error)+T*Ki*SUM_ERROR;//+(Kd/T)*(error-PREV_ERROR);
	//2.55 Fordi motorbox tar verdier fra 0 til 255
	
	
	//threshold
	if(abs(error) <= 3 ){
		u = 0;
	}
	
	//printf("REF: %i \n",(uint8_t)scaledRef);
	printf("Ref: %d EncoderValue: %d  u: %i \n", (uint8_t)scaledRef,(int)scaledEncoder, u);
	if(error>0){
		motor_dirRight();
	}else{
		motor_dirLeft();
	}
	if(u<250){
		motor_setVoltage(u); //input to motor
	}else{
		motor_setVoltage(150); //Max
	}
	
	SUM_ERROR = SUM_ERROR + error;
	PREV_ERROR = error;
}
