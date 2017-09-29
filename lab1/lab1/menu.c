/*
 * menu.c
 *
 * Created: 29.09.2017 15:03:33
 *  Author: nicolabc
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>


#include "menu.h"
#include "oled.h"
#include "joy.h"

node_t* currentSelection;
node_t* currentHead;

void menu_init(void){
	//oled_print("Linked list <--",4,0);
	
	node_t* newgame = malloc(sizeof(node_t)); //For å allokere nok minne av størrelse node_t
	node_t* highscore = malloc(sizeof(node_t));
	node_t* easy = malloc(sizeof(node_t));
	node_t* hard = malloc(sizeof(node_t));
	
	currentHead = newgame;
	//currentSelection = DER PILEN DIN PEKER NÅ
	
	newgame->name = "New game";
	newgame->previous = NULL;
	
	newgame->next = highscore;
	
	highscore->previous = newgame;
	highscore->next = NULL;
	
}

void defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous){
	
}

void printMenu(node_t* firstNode){
	
	while(1);
}

//void funksjon som kjører hele menyen?