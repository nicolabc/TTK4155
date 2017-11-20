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

/*Initializes CAN*/
void can_init();

/*Sends a can message, which is a struct*/
void can_send_message(can_msg* send);

/*Receives a can message struct*/
int can_receive_message(can_msg *wholeMessage);


#endif /* CAN_H_ */