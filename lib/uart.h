/*
 * uart.h
 *
 * Created: 04.09.2017 14:52:05
 *  Author: nicolabc
 */ 


#ifndef UART_H_
#define UART_H_


void USART_Init( unsigned int ubrr );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );

#endif /* UART_H_ */

