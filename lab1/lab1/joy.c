/*
 * joy.c
 *
 * Created: 18.09.2017 12:07:41
 *  Author: nicolabc
 */ 

#include "joy.h"
#include <avr/io.h>


int joy_getPercent(int raw){
	return (((raw*(1.0)-127.5) / 127.5) * 100);
}

int joy_getDirectionX(int raw){
	if (raw > 122 && raw < 133){
		return NEUTRAL;
	}
	else if(raw >= 133){
		return RIGHT;
	}
	return LEFT;
}

int joy_getDirectionY(int raw){
	if (raw > 122 && raw < 133){
		return NEUTRAL;
	}
	else if(raw >= 133){
		return UP;
	}
	return DOWN;
}

int joy_readButton(int button){
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