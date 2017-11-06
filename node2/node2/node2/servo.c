/*
 * servo.c
 *
 * Created: 23.10.2017 11:55:20
 *  Author: nicolabc
 */ 

#include "../../../lib/joy.h"
#include "timer.h"
#include "servo.h"



void servo_positionUpdate(int rawJoystickValue){
	int percent = joy_getPercent(rawJoystickValue);
	
	if(percent>=0 && percent<=100){
		timer_dutyCycleUpdate(percent);
	}
	
}
