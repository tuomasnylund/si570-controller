/*
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
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile char    uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint8_t uart_rx_buffer_index;

volatile char    uart_tx_buffer[UART_TX_BUFFER_SIZE];
volatile uint8_t uart_tx_buffer_index;

void uart_init(void){
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);

    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;

    //init buffers
    uart_rx_buffer_index = 0;

    RX_LED_DDR |= (1<<RX_LED_BIT);
}

int uart_getchar(FILE *stream){
    int retval;
    int i;

    while(uart_rx_buffer_index == 0);
      //  return _FDEV_EOF;

    cli();

    retval = uart_rx_buffer[0];

    for (i=0 ; i<uart_rx_buffer_index ; i++){
        uart_rx_buffer[i] = uart_rx_buffer[i+1];
    }
    uart_rx_buffer_index--;

    sei();

    return retval;
}

int uart_putchar(char c, FILE *stream){
    if (c == '\n')
        uart_putchar('\r',stream);

    if (uart_tx_buffer_index >= UART_TX_BUFFER_SIZE)
        return -1;

    cli();
    if(UCSR0A & (1<<UDRE0)){
        UDR0 = c;
    }
    else{
        uart_tx_buffer[uart_tx_buffer_index++] = c;
    }
    sei();


    return 0;
}

ISR(USART_RX_vect){
    char received = UDR0;
    uart_rx_buffer[uart_rx_buffer_index] = received;
    uart_rx_buffer_index++;
}

ISR(USART_TX_vect){
    int i;
    if (uart_tx_buffer_index == 0)
        return;

    UDR0 = uart_tx_buffer[0];
    for (i=0 ; i<uart_tx_buffer_index ; i++){
        uart_tx_buffer[i] = uart_tx_buffer[i+1];
    }
    uart_tx_buffer_index--;
}
