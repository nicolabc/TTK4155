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

typedef struct can_message {
	unsigned int id;
	uint8_t data[8];
	uint8_t length;
} can_msg;

void can_init();
void can_send_message(unsigned int identifier, uint8_t *message, uint8_t lengthOfData);
can_msg can_receive_message();


int can_error(); //returnerer 1 om vi har en error, 0 hvis ikke
int can_transmit_complete(); //om TXB0CTRL.TXREQ er lav, vil transmission v�re ferdig. returnerer 1 om vi har completed transmission. N�dvendig funksjon?
void can_int_vect(); //hva skal denne?


#endif /* CAN_H_ */