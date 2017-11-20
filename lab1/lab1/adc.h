/*
 * adc.h
 *
 * Created: 18.09.2017 09:43:26
 *  Author: nicolabc
 */ 


#ifndef ADC_H_
#define ADC_H_


/*Sets the relevant pins to input/output*/
void adc_init(void); 

/*Returns the value of the specified channel in the Multifunction board*/
uint8_t adc_read(uint8_t ch); //ch = 0: joystick x. ch = 1: joystick y. ch = 2: slider left. ch = 3: slider right.


#endif /* ADC_H_ */