/*
 * internalADC.h
 *
 * Created: 27.10.2017 16:39:27
 *  Author: nicolabc
 */ 


#ifndef INTERNALADC_H_
#define INTERNALADC_H_

/*Initializes the relevant pins to input/output*/
void internalADC_init(void);

/*Obtain a numeric result of the ADC conversion*/
uint16_t internalADC_getResult(void);

/*Starts a conversion*/
void internalADC_startConversion(void);
#endif /* INTERNALADC_H_ */