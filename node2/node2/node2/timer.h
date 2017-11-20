/*
 * timer.h
 *
 * Created: 23.10.2017 08:58:06
 *  Author: nicolabc
 */ 


#ifndef TIMER_H_
#define TIMER_H_


/*Initializes relevant pins to input/output*/
void timer_init();

/*Timer interrupt for node 2*/
void timer_timedInterrupt(void); //For PID update clock

/*Update the duty cycle value of the PWM signal*/
int timer_dutyCycleUpdate(int percent);

#endif /* TIMER_H_ */