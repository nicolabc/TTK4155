/*
 * motor.h
 *
 * Created: 03.11.2017 15:00:22
 *  Author: danieta
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_


void motor_init(void);
void motor_setVoltage(uint8_t myValue);
void motor_dirLeft(void);
void motor_dirRight(void);

#endif /* MOTOR_H_ */