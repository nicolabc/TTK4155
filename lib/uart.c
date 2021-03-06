/*
 * uart.c
 *
 * Created: 04.09.2017 14:52:15
 *  Author: nicolabc
 */ 
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"


void USART_Init( unsigned int ubrr )
 {
	 /* Set baud rate */
	 UBRR0H = (unsigned char)(ubrr>>8);
	 UBRR0L = (unsigned char)ubrr;

	 /* Enable receiver and transmitter*/
	 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	 
	 fdevopen(*USART_Transmit, *USART_Receive);
 }
 
 void USART_Transmit( unsigned char data )
 {
	 /* Wait for empty transmit buffer */
	 while( !( UCSR0A & (1<<UDRE0)) );
	 
	 /* Put data into buffer, sends the data */
	 UDR0 = data;
 }

 unsigned char USART_Receive( void )
 {
	 /*Wait for data to be received*/
	 while( !(UCSR0A & (1<<RXC0)) );
	 
	 /* Get and return received data from buffer*/
	 return UDR0;
 }
