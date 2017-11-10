/*
 * timer.h
 *
 * Created: 23.10.2017 08:58:06
 *  Author: nicolabc
 */ 


#ifndef TIMER_H_
#define TIMER_H_



void timer_init();
void timer_timedInterrupt(void); //For PID update clock
int timer_dutyCycleUpdate(int percent);

#endif /* TIMER_H_ */