/*
 * can.c
 *
 * Created: 09.10.2017 13:31:16
 *  Author: danieta
 */ 

 #include "can.h"
 #include "MCP2515.h"
 #include <avr/interrupt.h>

 #define set_bit( reg, bit ) (reg |= (1 << bit))
 #define clear_bit( reg, bit ) (reg &= ~(1 << bit))


 void can_init(){
	mcp2515_init(); //denne kaller på reset, som setter systemet i config mode
	
	
	//aktiverer interrupts (interrupt enable) for transmit og receive buffers, s.51
	mcp2515_bit_modify(MCP_CANINTE, 0b00000011, 0b00000011); 
	
	//Setter alle maske-bitene i standard identifier til 0, fordi da slippes alle messages gjennom. s. 35 (register 4-14 til 4-17), og s. 32, table 4-2
	mcp2515_write(MCP_RXM0SIDH, 0b00000000);
	mcp2515_write(MCP_RXM0SIDL, 0b00000000);
	mcp2515_write(MCP_RXM1SIDH, 0b00000000);
	mcp2515_write(MCP_RXM1SIDL, 0b00000000);
	
	//setter i normal mode, s. 60
	mcp2515_bit_modify(MCP_CANCTRL, 0b11100000, 0b00000000);
	
	#ifdef __AVR__ATmega162__
	cli();
	GICR |= (1 << INT2);
	sei();
	#endif
	
 }

void can_send_message(can_msg* send){//unsigned int identifier, uint8_t *message, uint8_t lengthOfData
	
	while((mcp2515_read(MCP_TXB0CTRL) & MCP_TXREQ)){ //så lenge transfer buffer 0 er fullt, vil MCP_TXREQ være høy, og vi er stuck i whilen. Når det er tomt, vil vi gå ut av whilen
	}
	
	//Left shifter 5 ganger for å lagre riktig i SID bufferregistrene
	mcp2515_write(MCP_TXB0SIDH+1, send->id<<5); //lagrer identifieren til messagen inn i første transferbuffer. Går fint å overskrive det som ligger der fordi det er 0 som må ligge der for standard identifier (ikke extended)
	
	 //Right shift 3 ganger for å flytte de 8 høyeste bit'ene til de 8 laveste, så de lagres i SIDH
	mcp2515_write(MCP_TXB0SIDH, send->id>>3); //lagrer identifieren til messagen inn i første transferbuffer
	
	mcp2515_write(MCP_TXB0SIDH+4, send->length); //lagrer datalengden i txb0dlc. plusser på 4 for å komme til txb0dlc (se tabell s. 63)

	//lagrer nå meldinga i TXB0D0-TXB0D6 (s. 63)
	if(send->length>8){
		printf("Length of data too big.");
		return;
	}
	int byte = 0;
	for (byte = 0; byte < send->length; byte++){
		mcp2515_write(MCP_TXB0SIDH+5+byte, send->data[byte]);
	}

	mcp2515_request_to_send(0); //sender RTS signal til TXB0 nå
}

int can_receive_message(can_msg *wholeMessage){
	wholeMessage->id = 0;
	wholeMessage->length = 0;
	wholeMessage->data[0] = 0b00000000;
	
	if (mcp2515_read(MCP_CANINTF) & MCP_RX0IF){ //om rx0if er høy, altså interrupt pinnen er høy, betyr det at receive buffer nr 0 har fått inn ny data
		unsigned int idH = mcp2515_read(MCP_RXB0SIDH);
		unsigned int idL = mcp2515_read(MCP_RXB0SIDH+1);
		idL = (idL >> 5); //Fordi de er idL er tre MSB i MCP_RXB0SIDL, må rightshifte 5 ganger
		idH = (idH << 3); //Må leftshifte 
		unsigned int id = (idH | idL); //Kan evt ta +
		
		
		uint8_t length = mcp2515_read(MCP_RXB0SIDH+4); //leser av rxb0dlc
		if (length > 8){
			printf("Length > 8, not allowed.");
			return 0;
		}
		
		//leser nå av rxb0d0 til rxb0d7 (avhengig av hvor lang meldinga vi har mottatt er), og lagrer det i arrayet message
		int byte = 0;
		//lagrer alt i en struct
		for (byte = 0; byte < length; byte++){
			wholeMessage->data[byte] = mcp2515_read(MCP_RXB0SIDH+5+byte);
		}
		
		
		wholeMessage->id = id;
		wholeMessage->length = length;
		
		mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0b00000000); //for å kunne reenable receive buffer 0 interrupten
	}
	
	
	
	return 1;
}



 //enable rollover
 //kap. 3, kap. 4, kap. 7