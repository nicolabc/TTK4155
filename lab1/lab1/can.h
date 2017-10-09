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

struct can_message {
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_msg;

void can_init();
void can_send_message(uint8_t identifier, uint8_t *message, uint8_t lengthOfData);
void can_error();
void can_transmit_complete(); //om TXB0CTRL.TXREQ er lav, vil transmission være ferdig
void can_data_receive();
void can_int_vect();


#endif /* CAN_H_ */