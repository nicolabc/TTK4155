/*
 * can.c
 *
 * Created: 09.10.2017 13:31:16
 *  Author: danieta
 */ 

 #include "can.h"
 #include "MCP2515.h"

 #define set_bit( reg, bit ) (reg |= (1 << bit))
 #define clear_bit( reg, bit ) (reg &= ~(1 << bit))


 void can_init(){
	mcp2515_init(); //denne kaller på reset, som setter systemet i config mode
	
	//setter i loopback mode, s. 60
	mcp2515_bit_modify(MCP_TXB0CTRL, MODE_MASK, MODE_LOOPBACK);
	
	//aktiverer interrupts (interrupt enable) for transmit og receive buffers, s.51
	mcp2515_bit_modify(MCP_CANINTE, 0b00000011, 0b00000011); 
	
	//Setter alle maske-bitene i standard identifier til 0, fordi da slippes alle messages gjennom. s. 35 (register 4-14 til 4-17), og s. 32, table 4-2
	mcp2515_write(MCP_RXM0SIDH, 0b00000000);
	mcp2515_write(MCP_RXM0SIDL, 0b00000000);
	mcp2515_write(MCP_RXM1SIDH, 0b00000000);
	mcp2515_write(MCP_RXM1SIDL, 0b00000000);

	

 }

void can_send_message(uint8_t identifier, uint8_t *message, uint8_t lengthOfData){
	//Kan legge til funksjon at vi først sjekker om et buffer (f.eks. txb0) er tomt, og deretter skrive. Om det ikke er tomt, skriver vi til et av de andre bufrene. 

	mcp2515_write(MCP_TXB0SIDH, identifier); //lagrer identifieren til messagen inn i første transferbuffer
	mcp2515_write(MCP_TXB0SIDH+4, lengthOfData); //lagrer datalengden i txb0dlc. plusser på 4 for å komme til txb0dlc (se tabell s. 63)

	//lagrer nå meldinga i TXB0D0-TXB0D6 (s. 63)

	int byte = 0;
	for (byte = 0; byte < lengthOfData; byte++){
		mcp2515_write(MCP_TXB0SIDH+5+byte, message[byte]);
	}

	mcp2515_request_to_send(0); //sender RTS signal til TXB0 nå
}


 //enable rollover
 //kap. 3, kap. 4, kap. 7