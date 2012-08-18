#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "pwm_16.h"

#define DIVIDER 32

void pwm16_set_volts(uint16_t millivolts){
    OCR1A = millivolts/DIVIDER;
}

void pwm16_set_amps   (uint16_t milliamps){
    OCR1B = milliamps/DIVIDER;
}

void pwm16_init(void){

    PWM_V_DDR |= (1<<PWM_V_BIT);
    PWM_A_DDR |= (1<<PWM_A_BIT);

    /** Set output comare to set on match, clear at top */
    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<COM1B1);

    /** Set to Fast PWM, TOP at ICR1 */
    TCCR1B |= (1<<WGM13);
    TCCR1B |= (1<<WGM12);
    TCCR1A |= (1<<WGM11);

    /** No clock prescaling */
    TCCR1B |= (1<<CS10);

    /** Init all of the registers */
    ICR1  = 0xFFFF/DIVIDER;
    OCR1A = 0;
    OCR1B = 0;
}
