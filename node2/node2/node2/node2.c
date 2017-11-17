/*
 * node2.c
 *
 * Created: 16.10.2017 13:20:09
 *  Author: nicolabc
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/sleep.h>
#include "../../../lib/uart.h"
#include "../../../lib/spi.h"
#include "../../../lib/MCP2515.h"
#include "../../../lib/can.h"
#include "timer.h"
#include "servo.h"
#include "internalADC.h"
#include "game.h"
#include "TWI_Master.h"
#include "dac.h"
#include "motor.h"
#include "encoder.h"
#include "solenoid.h"

#define F_CPU 16000000
#define FOSC 16000000// Clock Speed

//Fra node 1
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


volatile int RECEIVE_BUFFER_INTERRUPT = 0;
volatile int TIMER_OVERFLOW_INTERRUPT = 0;
volatile int ADC_CONVERSION_COMPLETE_INTERRUPT = 0;
volatile int16_t ENCODERVALUE = 0;
volatile int CAN_FIRST_MESSAGE_RECEIVED = 0;
volatile int CAN_CALIBRATION_NEEDED = 0;
volatile int REF_TO_MOTOR_PID = 0;


volatile double KP;

int main(void)
{
	cli();
	USART_Init(MYUBRR);
	can_init();
	timer_init();
	internalADC_init();
	TWI_Master_Initialise();
	motor_init();
	encoder_init();
	solenoid_init();
	
	
	/*set_sleep_mode(0);
	sleep_mode();
	sleep_enable();
	sei();
	sleep_cpu();*/
	//sleep_disable();
	
	
	sei(); //Global interrupt enable
	//motor_calibrate();
	
	can_msg melding;
	melding.id = 5;
	melding.length = 8;
	melding.data[0] = (uint8_t)('1');
	melding.data[1] = (uint8_t)('2');
	melding.data[2] = (uint8_t)('3');
	melding.data[3] = (uint8_t)('4');
	melding.data[4] = (uint8_t)('5');
	melding.data[5] = (uint8_t)('6');
	melding.data[6] = (uint8_t)('7');
	melding.data[7] = (uint8_t)('8');
	
	uint16_t result;
	
	int mottatt_data_char0 = 50; //startposisjon
	
	
	
    while(1)
    {
		/*//Interrupt
		if(//TIMERINTRERRUPT (MÅ OGSÅ VEKKE FRA SLEEP)
		
		//Sleep
		*/
		if(ADC_CONVERSION_COMPLETE_INTERRUPT){
			
			uint8_t gameOver = game_isGameOver();
			//printf("Game Over = %i \n", gameOver);
			
			//sender can melding som har info om spillet er over
			can_msg gameInfo;
			gameInfo.id = 5;
			gameInfo.length = 1; //skal kun sende over én verdi, som er verdien gameOver
			gameInfo.data[0] = gameOver;
			can_send_message(&gameInfo);

			
			internalADC_startConversion(); //Starte ny interrupt
			
			ADC_CONVERSION_COMPLETE_INTERRUPT = 0;
		}
		
		//	can_send_message(&melding);
		can_msg mottatt;
		
		
		//sjekker om receive bufre inneholder noe. se s. 69 i mcp2515
		volatile uint8_t statusReg = mcp2515_read_status();
	
		if(test_bit(statusReg, 0)){ //Mulig å lage det som en funskjon i ettertid
			
			//---------------CAN----------------------
			can_receive_message(&mottatt);		
			
			if(mottatt.id == 2){
				KP = mottatt.data[0];
				printf("Custom Kp = %i \n",(int)KP);
			}
			
			
			/*-------IF CAN MESSAGE IS JOYSTICK INFO-------*/
			if(mottatt.id == 1){
				mottatt_data_char0 = mottatt.data[0]; //X-akse
				int mottatt_data_char1 = mottatt.data[1];
				int mottatt_data_char2 = mottatt.data[2];
				int mottatt_data_char3 = mottatt.data[3];
				int mottatt_data_char4 = mottatt.data[4]; //Høyre knapp
				int mottatt_data_char5 = mottatt.data[5]; //Gamestatus
				int mottatt_data_char6 = mottatt.data[6];
				
				/*------------- VALUES TO MOTOR ------------------------*/
				CAN_CALIBRATION_NEEDED +=1;
				REF_TO_MOTOR_PID = joy_convertToPercentage(mottatt_data_char0,CAN_CALIBRATION_NEEDED); //for x-akse på joystick
				ENCODERVALUE = encoder_read(); //Leser verdi på encoderen
				CAN_FIRST_MESSAGE_RECEIVED = 1;
				
				/*----------------TO SERVO -------------------*/
				servo_positionUpdate(255-mottatt_data_char6);
				
				/*---------------SOLENOID-----------------*/
				if(mottatt_data_char4 == 1){
					
					solenoid_shoot();
				}
				printf("ID: %i  LENGTH: %i   ALL DATA  %i    %i   %i    %i    %i    %i    %i   \n", mottatt.id , mottatt.length, mottatt_data_char0, mottatt_data_char1, mottatt_data_char2, mottatt_data_char3, mottatt_data_char4, mottatt_data_char5, mottatt_data_char6);
				
			}
			
			
			
			
			RECEIVE_BUFFER_INTERRUPT = 0; //clearer interruptflagget
			
			
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
			
			
		}
		/*if(TIMER_COMPARE_MATCH == 1){
			
			TIMER_COMPARE_MATCH = 0;
		}*/
		
	}
}

ISR(INT2_vect){
	RECEIVE_BUFFER_INTERRUPT = 1;
	
}

ISR(ADC_vect){
	ADC_CONVERSION_COMPLETE_INTERRUPT = 1;
}


//Timer interrupt vector for sleep of program
ISR(TIMER3_COMPA_vect){
	//sleep_disable();
	if(CAN_FIRST_MESSAGE_RECEIVED == 1){
		motor_PIDspeed(REF_TO_MOTOR_PID,ENCODERVALUE);  ////Oppdater PID og spenning til motor (u)
	}
}