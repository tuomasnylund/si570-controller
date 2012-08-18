#ifndef _ADC_H_
#define _ADC_H_

#define ADC_MUX_CURRENT 6
#define ADC_MUX_VOLTAGE 7


void adc_init(void);
uint16_t adc_get_voltage(void);
uint16_t adc_get_current(void);

#endif
