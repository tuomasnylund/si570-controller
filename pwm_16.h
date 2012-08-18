#ifndef _PWM_16_H
#define _PWM_16_H

#define PWM_V_PORT PORTB
#define PWM_V_DDR  DDRB
#define PWM_V_BIT  PD1

#define PWM_A_PORT PORTB
#define PWM_A_DDR  DDRB
#define PWM_A_BIT  PB2

void pwm16_init(void);
void pwm16_set_amps  (uint16_t milliamps);
void pwm16_set_volts (uint16_t millivolts);

#endif
