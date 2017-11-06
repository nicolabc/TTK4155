/*
 * encoder.c
 *
 * Created: 06.11.2017 10:06:10
 *  Author: nicolabc
 */ 

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

uint16_t encoder_read(void){
	
	uint8_t MSB;
	uint8_t LSB;
	uint16_t encoderValue;
	
	int temp_1=0,temp_2=0,loop_var=0;
	uint8_t read;
	//clear_bit(PORTH,PH5); //Enable counter on MJ2 (!OE) ---- Set !OE low to enable output of encoder
	
	clear_bit(PORTH,PH3); //Set SEL low to get high byte
	
	_delay_us(30); //Wait 20 microseconds
	
	temp_1 = PINK; //Read 8MSB from encoder
	
	for(loop_var=0;loop_var<= 7;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |=  1<< (8-loop_var);
		} else
		{
			temp_2 &= ~((1<<(8-loop_var)));
		}
		
	}
	
	MSB = temp_2; 

	set_bit(PORTH,PH3); //Set SEL high to get low byte
	
	_delay_us(30); //Wait about 20 microseconds
	
	temp_1 = PINK; //Read 8LSB from encoder
	for(loop_var=0;loop_var<= 7;loop_var++)
	{
		read = temp_1 & (1<<loop_var);
		if(read){
			temp_2 |=  1<< (8-loop_var);
		} else
		{
			temp_2 &= ~((1<<(8-loop_var)));
		}
		
	}
	LSB = temp_2;
	
/*
	//Reset encoder
	clear_bit(PORTH,PH6); //Reset encoder counter (!RST)
	_delay_us(50); //Safety margin before starting counter
	set_bit(PORTH,PH6); //Enable encoder counter (!RST)
	*/
	//set_bit(PORTH,PH5); //Disable output of encoder
	
	encoderValue =(uint16_t) ((MSB<<8) | LSB);
	
	return encoderValue;
}