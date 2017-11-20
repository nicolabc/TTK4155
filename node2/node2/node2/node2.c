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
#include "../../../lib/joy.h"
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

/*----------GLOBAL VARIABLES----------*/
volatile int TIMER_OVERFLOW_INTERRUPT = 0;
volatile int ADC_CONVERSION_COMPLETE_INTERRUPT = 0;
volatile int16_t ENCODERVALUE = 0;
volatile int CAN_FIRST_MESSAGE_RECEIVED = 0;
volatile int CAN_CALIBRATION_NEEDED = 0;
volatile int REF_TO_MOTOR_PID = 0;
volatile int HAVE_ENTERED_CUSTOM = 0;
volatile int RECEIVED_GAMESTATUS = 0; //Default MENU

/*----------CUSTOM CONTROLLER PARAMETERS----------*/
volatile int KP;
volatile int KI;
volatile int KD;


int main(void)
{
	/*----------INITIALIZATION----------*/
	cli();
	USART_Init(MYUBRR);
	can_init();
	timer_init();
	internalADC_init();
	TWI_Master_Initialise();
	motor_init();
	encoder_init();
	solenoid_init();
	
	/*---------Init of sleep---------------*/
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	
	sei(); //Global interrupt enable
	
	/*---------MAIN BODY OF THE GAME---------------*/
    while(1)
    {
		
		if(ADC_CONVERSION_COMPLETE_INTERRUPT){
			uint8_t gameOver = game_isGameOver();
			
			/*---------SENDS CAN MESSAGE TO NOTIFY IF THE GAME IS OVER---------------*/
			can_msg gameInfo;
			gameInfo.id = 5;
			gameInfo.length = 1; //skal kun sende over én verdi, som er verdien gameOver
			gameInfo.data[0] = gameOver;
			can_send_message(&gameInfo);

			/*---------START CONVERSION AND CLEAR INTERRUPT FLAG---------------*/
			internalADC_startConversion(); //Starte ny interrupt
			ADC_CONVERSION_COMPLETE_INTERRUPT = 0;
		}
		
		can_msg mottatt; //RECEIVED CAN MESSAGE FROM NODE 1
		
		
		volatile uint8_t statusReg = mcp2515_read_status(); //CHECKS THE STATUS REGISTER
	
		/*------------IF THE RECEIVE BUFFER INTERRUPT FLAG IS SET-----------*/
		if(test_bit(statusReg, 0)){ 
			can_receive_message(&mottatt);		
			
			/*-------IF CAN MESSAGE IS CUSTOM CONTROLLER INFO-------*/
			if(mottatt.id == 100){ 
				HAVE_ENTERED_CUSTOM = 1; //Set flag high that we have customized the parameters
				KP = mottatt.data[0];
				KI = mottatt.data[1];
				KD = mottatt.data[2];
				printf("Custom Kp = %i, Custom Ki = %i, Custom Kd = %i \n",KP,KI,KD);
			}
			
			
			/*-------IF CAN MESSAGE IS JOYSTICK INFO-------*/
			if(mottatt.id == 1){
				int mottatt_data_char0 = mottatt.data[0]; //X-akse
				int mottatt_data_char1 = mottatt.data[1];
				int mottatt_data_char2 = mottatt.data[2];
				int mottatt_data_char3 = mottatt.data[3];
				int mottatt_data_char4 = mottatt.data[4]; //Høyre knapp
				RECEIVED_GAMESTATUS = mottatt.data[5]; //Gamestatus
				int mottatt_data_char6 = mottatt.data[6];
				
				/*------------- VALUES TO MOTOR ------------------------*/
				CAN_CALIBRATION_NEEDED +=1; //If the value is equal to 1, we set the midpoint in the conversion to percentage
				REF_TO_MOTOR_PID = joy_convertToPercentage(mottatt_data_char0,CAN_CALIBRATION_NEEDED); //for x-akse på joystick
				ENCODERVALUE = encoder_read(); //Leser verdi på encoderen
				CAN_FIRST_MESSAGE_RECEIVED = 1;
				
				/*----------------CHANGE BEHAVIOUR OF SERVO ACCORDING TO GAME STATUS-------------------*/
				if(RECEIVED_GAMESTATUS == PLAYING_EASY){
					servo_positionUpdate(255-mottatt_data_char0);
				}
				else if(RECEIVED_GAMESTATUS == PLAYING_HARD){
					servo_positionUpdate(mottatt_data_char0);
				}
				else if (RECEIVED_GAMESTATUS == PLAYING_NORMAL || RECEIVED_GAMESTATUS == PLAYING_CUSTOM)
				{
					servo_positionUpdate(255-mottatt_data_char6);
				}
				else{
					//Do nothing. Do not update the servo position when not playing 
				}
				
				
				/*---------------TO SHOOT THE SOLENOID-----------------*/
				if(mottatt_data_char4 == 1 && RECEIVED_GAMESTATUS != GAMEOVER){
					solenoid_shoot();
				}
				
			}	
			mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //SET THE RECEIVE BUFFER INTERRUPT FLAG LOW AFTER HANDLING THE INTERRUPT
		}
		
		sleep_mode(); //The MCU wakes up by an interrupt routine
		
	}
}


ISR(ADC_vect){
	ADC_CONVERSION_COMPLETE_INTERRUPT = 1;
}


ISR(TIMER3_COMPA_vect){
	if(CAN_FIRST_MESSAGE_RECEIVED == 1){
		if (RECEIVED_GAMESTATUS == PLAYING_CUSTOM) //If we have customized the parameters
		{
			motor_PIDspeed(REF_TO_MOTOR_PID,ENCODERVALUE,KP,KI,KD);
		}
		else if (RECEIVED_GAMESTATUS == PLAYING_EASY) 
		{
			motor_PIDspeed(REF_TO_MOTOR_PID,ENCODERVALUE,120,50,4);
		}
		else if (RECEIVED_GAMESTATUS == PLAYING_NORMAL) 
		{
			
			motor_PIDspeed(REF_TO_MOTOR_PID,ENCODERVALUE,120,50,4);
		}
		else if (RECEIVED_GAMESTATUS == PLAYING_HARD){
			
			motor_PIDspeed(REF_TO_MOTOR_PID,ENCODERVALUE,0,100,100);  //Oppdater PID og spenning til motor (u)
		}
		else{
			printf("Game over\n");
			motor_PIDspeed(0,0,0,0,0); 
		}
	}
}