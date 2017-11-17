/*
 * game.c
 *
 * Created: 30.10.2017 11:49:52
 *  Author: nicolabc
 */ 
#include <stdint.h>

#include "game.h"
#include "internalADC.h"



int game_isGameOver(void){
	uint16_t adcValue = internalADC_getResult();
	//printf("ADC RESULT IR: %i \n", adcValue);
	if(adcValue<= 300){ //Endre denne for IR sensorsensitivity
		return 1;
	}
	return 0;
}