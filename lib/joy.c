/*
 * joy.c
 *
 * Created: 18.09.2017 12:07:41
 *  Author: nicolabc
 */ 

#include "joy.h"
#include "../lab1/lab1/menu.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>


int joyLastDirectionY; //M� gj�res global for � leve etter vi g�r ut av scope
int joyLastDirectionX;
volatile int MID_VALUE;



int joy_convertToPercentage(int adcValue, int isFirstMessage){
	if(isFirstMessage == 1){
		MID_VALUE = adcValue;
	}
	
	if(adcValue > MID_VALUE){
		adcValue = 100*(adcValue-MID_VALUE)/(255-MID_VALUE);
		}else if(adcValue<MID_VALUE){
		adcValue = 100*(adcValue-MID_VALUE)/MID_VALUE;
		}else{
		adcValue = 0;
	}
	return adcValue;
	
}
int joy_getPercent(int raw){
	return (((raw*(1.0)) / 255) * 100);
}

int joy_getDirectionX(int raw){
	
	if (raw > 28 && raw < 228){
		return NEUTRAL;
	}
	else if(raw >= 228){
		return RIGHT;
	}
	return LEFT;
}

int joy_getDirectionY(int raw){

	if (raw > 8 && raw < 248){ //127.5+-19.5
		return NEUTRAL;
	}
	else if(raw >= 248){
		return UP;
	}
	return DOWN;
}

int joy_readButton(int button){ // Button 0, 1 or 2
	if (button == 2){
		if ((PINB & (1<<(PB0+button))) > 0){
			return 0; //s�: om joystickknappen ikke er trykket inn, vil verdien i parentesen v�re 4. om knappen ikke er trykket inn, returnerer vi 0
		}
		return 1; //ellers returnerer vi 1
	}
	if (button == 1){
		if ((PINB & (1<<(PB0+button))) > 0){
			return 1; //s� om h�yreknappen er trykket inn, vil verdien i parentesen v�re 2. Og da returnerer vi 1
		}
		return 0;
	}
	return (PINB & (1<<(PB0+button)));
}

int joy_doesDirectionChange(void){
	int joyCurrentDirectionY = joy_getDirectionY(adc_read(1));
	int joyCurrentDirectionX = joy_getDirectionX(adc_read(0));
	int joyCurrentDirection = NEUTRAL;
	
	int doesDirectionChange = 0;
	
	if(joyLastDirectionY != joyCurrentDirectionY){ //Prioriterer opp og ned
		joyCurrentDirection = joyCurrentDirectionY;
		oled_clear_screen(); //Skjermen m� oppdateres
		doesDirectionChange = 1;
		
	}
	else if(joyLastDirectionX != joyCurrentDirectionX){
		joyCurrentDirection = joyCurrentDirectionX;
		oled_clear_screen(); //skjermen m� oppdateres
		doesDirectionChange = 1;
	}
	else{
		doesDirectionChange = 0;
	}
	switch(joyCurrentDirection){
		case DOWN:
			if(currentSelection->next != NULL){
				currentSelection = currentSelection->next;
			}
				
			break;
		case UP:
			if(currentSelection->previous != NULL){
				currentSelection = currentSelection->previous;
			}
			break;
		case RIGHT:
			if(currentSelection->child != NULL){
				currentSelection = currentSelection->child;
				currentHead = currentSelection;
			}
			else if(currentSelection->name == "Easy"){
				GAMESTATUS = PLAYING_EASY;
				
				
			}
			else if(currentSelection->name == "Normal"){
				GAMESTATUS = PLAYING_NORMAL;
			
				
			}
			else if(currentSelection->name == "Hard"){
				GAMESTATUS = PLAYING_HARD;
				
			}else if(currentSelection->name == "Custom"){
				GAMESTATUS = PLAYING_CUSTOM;
			}
			
			break;
		case LEFT:
			if(currentSelection->parent != NULL){
				currentSelection = currentSelection->parent;
				currentHead = currentSelection;
			}
			break;
		case NEUTRAL:
			break;
			
	}
	joyLastDirectionY = joyCurrentDirectionY; //Oppdaterer hva som var sist
	joyLastDirectionX = joyCurrentDirectionX;
	
	return doesDirectionChange;
}

