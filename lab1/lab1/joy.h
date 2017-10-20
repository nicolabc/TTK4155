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

struct joystickinfo {
	int x;
	int y;
	int dir;
} joystick;

int joy_getPercent(int raw);

int joy_getDirectionX(int raw);

int joy_getDirectionY(int raw);

int joy_readButton(int button);

int joy_doesDirectionChange(void);

//int joy_updateMultiboardInfo(can_msg *yourMessage);

#endif /* JOY_H_ */