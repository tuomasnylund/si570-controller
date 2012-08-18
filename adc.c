#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"

void adc_init(void){
    ADMUX |= (1<<REFS0); //vref AVCC
    //ADCSRA |= (1<<ADEN) | (1<<ADIE);
    ADCSRA |= (1<<ADEN);
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1); //ADC prescaler 64. 8MHz/64 = 125Khz
}

static inline void adc_set_mux(uint8_t channel){
    ADMUX &= ~(0b1111);
    ADMUX |= channel;
}
uint16_t adc_get_voltage(void){
   adc_set_mux(ADC_MUX_VOLTAGE);
   ADCSRA |= (1<<ADSC);
   while(!(ADCSRA & (1<<ADIF)));

   ADCSRA |= (1<<ADIF);

   return ADC;
}
uint16_t adc_get_current(void){
   adc_set_mux(ADC_MUX_CURRENT);
   ADCSRA |= (1<<ADSC);
   while(!(ADCSRA & (1<<ADIF)));

   ADCSRA |= (1<<ADIF);

   return ADC;
}
