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
	
	
	node_t* newgame = malloc(sizeof(node_t)); //For å allokere nok minne av størrelse node_t
	node_t* highscore = malloc(sizeof(node_t));
	node_t* options = malloc(sizeof(node_t));
	node_t* credits = malloc(sizeof(node_t));
	node_t* developers = malloc(sizeof(node_t));
	node_t* easy = malloc(sizeof(node_t));
	node_t* normal = malloc(sizeof(node_t));
	node_t* hard = malloc(sizeof(node_t));
	currentHead = malloc(sizeof(node_t));
	currentSelection = malloc(sizeof(node_t));
	
	currentHead = newgame;
	//currentSelection = DER PILEN DIN PEKER NÅ
	currentSelection = newgame;
	
	defineMenuEntries(newgame,"New game",highscore,NULL,easy,NULL);
	defineMenuEntries(highscore,"Highscore",options,newgame,NULL,NULL);
	defineMenuEntries(options,"Options",credits,highscore,NULL,NULL);
	defineMenuEntries(credits,"Credits",NULL,options, developers, NULL);
	
	defineMenuEntries(easy,"Easy",normal,NULL,NULL,newgame);
	defineMenuEntries(normal, "Normal", hard, easy, NULL,newgame);
	defineMenuEntries(hard,"Hard",NULL,normal,NULL,newgame);
	
	defineMenuEntries(developers,"Daniel Tavakoli          Nicolas Carbone          Byggern Autumn 2017",NULL,NULL,NULL,newgame);
		
}

void defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous, node_t* submenuHead, node_t* parentmenuHead){
	myNode->name = nodeName;
	myNode->previous = previous;
	myNode->next = next;
	myNode->submenuHead = submenuHead;
	myNode->parentmenuHead = parentmenuHead;
}


void printMenu(void){ //Argument node_t* firstNode?
	
	node_t* temp = malloc(sizeof(node_t)); //For å iterere gjennom den lenkede listen
	temp = currentHead;
	
	int page = 0;
	while(temp!= NULL){ //Mens noden eksisterer
		
		
		if(currentSelection == temp){
			printf("TRUE \n");
			oled_print("<--",page,50); //Burde pilen hardkodes et sted slik eller legges rett etter skriften?
		}
		oled_print(temp->name, page,0);
		temp = temp->next;
		page++;
	}
}