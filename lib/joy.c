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


int joyLastDirectionY; //Må gjøres global for å leve etter vi går ut av scope
int joyLastDirectionX;

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
			return 0; //så: om joystickknappen ikke er trykket inn, vil verdien i parentesen være 4. om knappen ikke er trykket inn, returnerer vi 0
		}
		return 1; //ellers returnerer vi 1
	}
	if (button == 1){
		if ((PINB & (1<<(PB0+button))) > 0){
			return 1; //så om høyreknappen er trykket inn, vil verdien i parentesen være 2. Og da returnerer vi 1
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
		oled_clear_screen(); //Skjermen må oppdateres
		doesDirectionChange = 1;
		
	}
	else if(joyLastDirectionX != joyCurrentDirectionX){
		joyCurrentDirection = joyCurrentDirectionX;
		oled_clear_screen(); //skjermen må oppdateres
		doesDirectionChange = 1;
	}
	else{
		doesDirectionChange = 0;
	}
	switch(joyCurrentDirection){
		case DOWN:
			//oled_print("DOWN", 6 , 70);
			if(currentSelection->next != NULL){
				currentSelection = currentSelection->next;
			}
				
			break;
		case UP:
			//oled_print("UP", 6,70);
			if(currentSelection->previous != NULL){
				currentSelection = currentSelection->previous;
			}
			break;
		case RIGHT:
			if(currentSelection->child != NULL){
				currentSelection = currentSelection->child;
				currentHead = currentSelection;
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
		//default:
				
		//default:
			
		//Burde ikke skje
			
	}
	joyLastDirectionY = joyCurrentDirectionY; //Oppdaterer hva som var sist
	joyLastDirectionX = joyCurrentDirectionX;
	
	return doesDirectionChange;
}

