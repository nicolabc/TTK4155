/*
 * externalmemory.c
 *
 * Created: 11.09.2017 13:09:05
 *  Author: nicolabc
 */ 
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#include "externalmemory.h"

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))

void extMem_init(void){
	
	set_bit(MCUCR,SRE); // MCUCR |= (1 << SRE); //Setter SRE høy i MCUCR registeret slik at AVR kan skrive til external memory
	set_bit(SFIOR,XMM2); // SFIOR |= (1 << XMM2); //Maskerer ut PC7-PC4 til JTAG
	
}