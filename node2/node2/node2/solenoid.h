/*
 * solenoid.h
 *
 * Created: 06.11.2017 13:11:55
 *  Author: nicolabc
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

/*Initializes relevant pins to input/output*/
void solenoid_init(void);

/*Generates an impulse on the solenoide and make it shoot*/
void solenoid_shoot(void);

#endif /* SOLENOID_H_ */