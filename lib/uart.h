/*
 * uart.h
 *
 * Created: 04.09.2017 14:52:05
 *  Author: nicolabc
 */ 


#ifndef UART_H_
#define UART_H_

/*Initializes the USART*/
void USART_Init(unsigned int ubrr);

/*Sends data into buffer*/
void USART_Transmit(unsigned char data);

/*Collects data from buffer*/
unsigned char USART_Receive(void);

#endif /* UART_H_ */

