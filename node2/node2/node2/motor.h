/*
 * motor.h
 *
 * Created: 03.11.2017 15:00:22
 *  Author: danieta
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

/*Initializes relevant pins to input/output*/
void motor_init(void);

/*Sets voltage of the motor*/
void motor_setVoltage(uint8_t myValue); 

/*Changes direction of the motor to left*/
void motor_dirLeft(void);

/*Changes direction of the motor to right*/
void motor_dirRight(void);

/*Implementation of the PID controller*/
void motor_PIDspeed(int velRef, int16_t encoderValue, int Kp_in, int Ki_in, int Kd_in);

#endif /* MOTOR_H_ */