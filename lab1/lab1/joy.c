/*
 * joy.c
 *
 * Created: 18.09.2017 12:07:41
 *  Author: nicolabc
 */ 

#include "joy.h"



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