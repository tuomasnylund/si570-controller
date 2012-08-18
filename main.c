/*
* File: main file for si570 example
* Author: Tuomas Nylund (tuomas.nylund@gmail.com)
* Website: http://tuomasnylund.fi
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
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
    double frequency;

    init();
    for(;;){
        fgets(buffer,BUFLEN,stdin);
        switch (buffer[0]){
            case 'i':
                si570_init();
                break;
            case 'p':
                printf("HS_DIV: %u\n",si570_get_hsdiv());
                _delay_ms(50);
                printf("N1: %u\n",si570_get_n1());
                _delay_ms(50);
                printf("RFREQ: %lu\n",si570_get_rfreq_truncated());
                break;
            case 'r':
                si570_read_registers();
                break;
            case 'w':
                si570_write_registers();
                break;
            case 'f':
                frequency = atof(&(buffer[1]));
                si570_set_frequency(frequency);
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
