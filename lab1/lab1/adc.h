/*
 * adc.h
 *
 * Created: 18.09.2017 09:43:26
 *  Author: nicolabc
 */ 


#ifndef ADC_H_
#define ADC_H_


void adc_init(void);

/*
int adc_getX(void);

int adc_getY(void);

int adc_getLeftSlider(void);

int adc_getRightSlider(void);
*/
//void adc_init();

uint8_t adc_read(uint8_t ch);



#endif /* ADC_H_ */