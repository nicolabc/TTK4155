/*
 * multiboardInfo.c
 *
 * Created: 20.10.2017 14:58:31
 *  Author: nicolabc
 */ 
#include "../../lib/can.h"
#include "../../lib/joy.h"
#include "adc.h"

int multiboardInfo_update(can_msg* yourMessage){
	
	yourMessage->id = 1; //Her bestemmer vi at en ID som 1 vil være en melding fra joysticken
	yourMessage->length = 7;
	yourMessage->data[0] = adc_read(0); //sender inn x-verdien til joysticken først. Denne verdien er mellom -100 og 100
	yourMessage->data[1] = adc_read(1); //y-verdien til joystick
	yourMessage->data[2] = joy_readButton(2); //leser joystick-knappen (L3 på ps4)
	yourMessage->data[3] = joy_readButton(0); //leser venstre knapp
	yourMessage->data[4] = joy_readButton(1); //leser høyre knapp
	yourMessage->data[5] = adc_read(2); //Venstre slider
	yourMessage->data[6] = adc_read(3); //Høyre slider
	
	return 1;
}