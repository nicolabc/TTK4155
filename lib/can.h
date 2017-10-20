/*
 * can.h
 *
 * Created: 09.10.2017 13:31:06
 *  Author: danieta
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>
#include <stdio.h>

typedef struct {
	unsigned int id;
	int data[8];
	uint8_t length;
} can_msg;

void can_init();
void can_send_message(can_msg* send);
int can_receive_message(can_msg *wholeMessage);

//Burde implementeres. Sjekk error-flagg på error-register i mcp2515, og om et error-flagg er høyt; returner 1. Er egentlig bare if-setninger
int can_error(); //returnerer 1 om vi har en error, 0 hvis ikke

//Disse kan være unødvendige. Ikke implementert foreløpig
int can_transmit_complete(); //om TXB0CTRL.TXREQ er lav, vil transmission være ferdig. returnerer 1 om vi har completed transmission. Nødvendig funksjon?
void can_int_vect(); //hva skal denne?


#endif /* CAN_H_ */