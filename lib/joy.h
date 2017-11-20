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
	GAMEOVER = 4,
	PLAYING_CUSTOM = 5
};

extern int GAMESTATUS;

struct joystickinfo {
	int x;
	int y;
	int dir;
} joystick;

/*Returns the joystick value in percent (-100% to 100%)*/
int joy_convertToPercentage(int adcValue, int isFirstMessage);

/*Converts from 0-255 to 0-100*/
int joy_getPercent(int raw);

/*Returns the X direction of the joystick*/
int joy_getDirectionX(int raw);

/*Returns the Y direction of the joystick*/
int joy_getDirectionY(int raw);

/*Returns the value of a specified button. 0 = not pressed, 1 = pressed*/
int joy_readButton(int button);

/*Detects if the joystick direction changes in X or Y direction, and updates the linked list for the menu*/
int joy_doesDirectionChange(void);


#endif /* JOY_H_ */