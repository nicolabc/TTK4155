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
	struct node* child; //om vi er i startsiden, så er f.eks. Easy en child. Child peker på øverste element i undermenyen til parent
	struct node* parent; //om vi er i easy-siden, så er startsiden parent
} node_t;

extern node_t* currentSelection; //Deklarerer med extern
extern node_t* currentHead;		//Definisjonen ligger i .c filen

void menu_init(void);
void menu_defineMenuEntries(node_t* myNode, char* nodeName, node_t* next, node_t* previous, node_t* child, node_t* parent);//, node_t* parent);
void menu_save(void);



#endif /* MENU_H_ */