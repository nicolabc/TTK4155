/*
 * joy.h
 *
 * Created: 18.09.2017 12:07:31
 *  Author: nicolabc
 */ 


#ifndef JOY_H_
#define JOY_H_

enum dir {
	LEFT = 0,
	RIGHT = 1,
	DOWN = 2,
	UP = 3,
	NEUTRAL = 4,
};

enum gamestate {
	MENU = 0,
	PLAYING_EASY = 1,
	PLAYING_NORMAL = 2,
	PLAYING_HARD = 3,
	GAMEOVER = 4
};

extern int GAMESTATUS;

struct joystickinfo {
	int x;
	int y;
	int dir;
} joystick;



int joy_getPercent(int raw);

int joy_getDirectionX(int raw);

int joy_getDirectionY(int raw);

int joy_readButton(int button);

int joy_doesDirectionChange(void);//HUSK OLDE_REFRESH FOR Å CLEARE SCREEN HVER GANG DU BYTTER PÅ SKJERMEN

//int joy_updateMultiboardInfo(can_msg *yourMessage);

#endif /* JOY_H_ */