/*
 * encoder.c
 *
 * Created: 06.11.2017 10:06:10
 *  Author: nicolabc
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "encoder.h"
#define set_bit( reg, bit )(reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))


void encoder_init(void){
	set_bit(DDRH,PH6); //Setter PH6 (!RST) til output
	set_bit(PORTH,PH6); //!RST til 1
	
	set_bit(DDRH,PH5); //Setter PH5 (!OE) til output
	clear_bit(PORTH,PH5); //Enable counter on MJ2 (!OE)
	
	set_bit(DDRH,PH3); //Setter PH3 til output(SEL)
	
	//Setter data bits til input
	clear_bit(DDRK, PK0);
	clear_bit(DDRK, PK1);
	clear_bit(DDRK, PK2);
	clear_bit(DDRK, PK3);
	clear_bit(DDRK, PK4);
	clear_bit(DDRK, PK5);
	clear_bit(DDRK, PK6);
	clear_bit(DDRK, PK7);
}

int16_t encoder_read(void){
	
	int16_t MSB = 0;
	int16_t LSB = 0;
	int16_t encoderValue = 0;
	
	//clear_bit(PORTH,PH5); //Enable counter on MJ2 (!OE) ---- Set !OE low to enable output of encoder
	
	clear_bit(PORTH,PH3); //Set SEL low to get high byte
	
	_delay_us(20); //Wait 20 microseconds
	
	encoderValue = (((int16_t)PINK) << 8); //Read 8MSB from encoder

	set_bit(PORTH,PH3); //Set SEL high to get low byte
	
	_delay_us(20); //Wait about 20 microseconds
	
	encoderValue |= PINK; //Read 8LSB from encoder
	
	
/*
	//Reset encoder
	clear_bit(PORTH,PH6); //Reset encoder counter (!RST)
	_delay_us(50); //Safety margin before starting counter
	set_bit(PORTH,PH6); //Enable encoder counter (!RST)
	*/
	//set_bit(PORTH,PH5); //Disable output of encoder
	
	//encoderValue =(int16_t) (MSB | LSB);
	
	return encoderValue;
}