/*
 * motor.h
 *
 * Created: 03.11.2017 15:00:22
 *  Author: danieta
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_


void motor_init(void);
void motor_setVoltage(uint8_t myValue); //Verdier fra 0 til 255. 255 er maks spenning og dermed maks fart
void motor_dirLeft(void);
void motor_dirRight(void);
void motor_calibrate(void);
void motor_PIDspeed(int velRef, int16_t encoderValue, int Kp_in, int Ki_in, int Kd_in);

#endif /* MOTOR_H_ */