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
	struct node* submenu;
} node_t;



void menu_init(void);
void defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous);
void printMenu(node_t* firstNode);



#endif /* MENU_H_ */