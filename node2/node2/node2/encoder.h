/*
 * encoder.h
 *
 * Created: 06.11.2017 10:05:42
 *  Author: nicolabc
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

/*Initializes relevant pins to input/output*/
void encoder_init(void);

/*Reads the values from the encoder*/
int16_t encoder_read(void);


#endif /* ENCODER_H_ */