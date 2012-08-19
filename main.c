#define F_CLOCK  8000000
#define F_CPU    8000000

#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include "i2c.h" 
#include "si570.h" 

#define LED_TEST_PORT PORTB
#define LED_TEST_DDR  DDRB
#define LED_TEST_BIT  PB0

#define BUFLEN 20

/**********************************************************
 * Function prototypes
 **********************************************************/
void init(void);

/**********************************************************
 * External variables
 **********************************************************/

/**********************************************************
 * Global variables
 **********************************************************/
static FILE mystdout = FDEV_SETUP_STREAM(
        uart_putchar,
        uart_getchar,
        _FDEV_SETUP_RW);

/**********************************************************
 * Main function
 **********************************************************/
int main(void){
    char buffer[BUFLEN];
    char ftoabuf[8];
    double frequency;

    init();
    for(;;){
        fgets(buffer,BUFLEN,stdin);
        switch (buffer[0]){
            case 'i':
                si570_init();
                break;
                /*
            case 'p':
                printf("HS_DIV: %u\n",si570_get_hsdiv());
                _delay_ms(50);
                printf("N1: %u\n",si570_get_n1());
                _delay_ms(50);
                printf("RFREQ: %lu\n",si570_get_rfreq_truncated());
                break;
                */
            case 'r':
                si570_read_registers();
                break;
            case 'w':
                si570_write_registers();
                break;
            case 'f':
                frequency = atof(&(buffer[1]));
                si570_set_frequency(4*frequency);
                puts(dtostrf(frequency,4,4,ftoabuf));
                break;
        }
    }
    return 0;
}

/**********************************************************
 * Other functions
 **********************************************************/
void init(void){
    uart_init();
    i2c_init();
    si570_init();

    LED_TEST_DDR |= (1<<LED_TEST_BIT);

    stdout = stdin = &mystdout;

    sei();
}
