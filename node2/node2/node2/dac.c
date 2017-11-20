/*
 * dac.c
 *
 * Created: 03.11.2017 14:40:44
 *  Author: danieta
 */ 

#include <avr/io.h>
#include "dac.h"


void dac_send(uint8_t data){
	uint8_t msg[3];
	msg[0] = 0b01011110; //Slave address byte
	msg[1] = 0x00; //Command byte
	msg[2] = data; //Output
	TWI_Start_Transceiver_With_Data(msg,3); //Sizeof
}