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
#include "../../lib/joy.h"
#include "sram.h"

node_t* currentSelection;
node_t* currentHead;
node_t* developers;

void menu_init(void){
	
	
	node_t* newgame = malloc(sizeof(node_t)); //For � allokere nok minne av st�rrelse node_t
		node_t* easy = malloc(sizeof(node_t));
		node_t* normal = malloc(sizeof(node_t));
		node_t* hard = malloc(sizeof(node_t));
		node_t* custom = malloc(sizeof(node_t));
	
	node_t* highscore = malloc(sizeof(node_t));
	node_t* options = malloc(sizeof(node_t));
		node_t* sound = malloc(sizeof(node_t));
			node_t* sound_on = malloc(sizeof(node_t));
			node_t* sound_off = malloc(sizeof(node_t));
		node_t* sensitivity = malloc(sizeof(node_t));
	
	node_t* credits = malloc(sizeof(node_t));
		developers = malloc(sizeof(node_t));
	
	
	
	currentHead = malloc(sizeof(node_t));
	currentSelection = malloc(sizeof(node_t));
	
	
	currentHead = newgame;

	currentSelection = newgame;	//currentSelection = DER PILEN DIN PEKER N�
	
	menu_defineMenuEntries(newgame,"New Game",highscore,NULL,easy,NULL);
		menu_defineMenuEntries(easy,"Easy",normal,NULL,NULL,newgame);
		menu_defineMenuEntries(normal, "Normal", hard, easy, NULL,newgame);
		menu_defineMenuEntries(hard,"Hard",custom,normal,NULL,newgame);
		menu_defineMenuEntries(custom,"Custom",NULL,hard,NULL,newgame);
	menu_defineMenuEntries(highscore,"Highscore",options,newgame,NULL,NULL);
	
	menu_defineMenuEntries(options,"Options",credits,highscore,sensitivity,NULL);
		menu_defineMenuEntries(sensitivity, "Sensitivity", sound, NULL, NULL, options);
		menu_defineMenuEntries(sound, "Sound", NULL, sensitivity, sound_on, options);
			menu_defineMenuEntries(sound_on, "Sound on", sound_off, NULL, NULL, sound);
			menu_defineMenuEntries(sound_off, "Sound off", NULL, sound_on, NULL, sound);
		
	
	menu_defineMenuEntries(credits,"Credits",NULL,options, developers, NULL);
		menu_defineMenuEntries(developers,"Daniel Tavakoli          Nicolas Carbone          Byggern Autumn 2017          � � �",NULL,NULL,NULL,credits);
		
}

void menu_defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous, node_t* child, node_t* parent){
	myNode->name = nodeName;
	myNode->previous = previous;
	myNode->next = next;
	myNode->child = child;
	myNode->parent = parent;
}


void menu_save(void){ //Argument node_t* firstNode?
	
	node_t* temp = malloc(sizeof(node_t)); //For � iterere gjennom den lenkede listen
	temp = currentHead;
	
	int page = 0;
	while(temp->previous!=NULL){ //Itererer oss bakover til f�rste node i listen
		temp = temp->previous;
	}
	while(temp!= NULL){ //Mens noden eksisterer
		
		
		if(currentSelection == temp && currentSelection != developers){
			sram_save_string("-->",page,0); 
		}
		sram_save_string(temp->name, page,20);
		
		temp = temp->next;
		page++;
	}
}

