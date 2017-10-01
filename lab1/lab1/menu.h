/*
 * menu.h
 *
 * Created: 29.09.2017 15:03:43
 *  Author: nicolabc
 */ 


#ifndef MENU_H_
#define MENU_H_

typedef struct node{
	char* name;
	struct node* next;
	struct node* previous;
	struct node* submenuHead;
	struct node* parentmenuHead;
} node_t;

extern node_t* currentSelection; //Deklarerer med extern
extern node_t* currentHead;		//Definisjonen ligger i .c filen

void menu_init(void);
void defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous, node_t* submenuHead, node_t* parentmenuHead);
void updateCurrentSelection(void);
void printMenu(void);



#endif /* MENU_H_ */