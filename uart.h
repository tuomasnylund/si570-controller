#ifndef _UART_H_
#define _UART_H_
#define F_CPU    8000000

#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define RX_LED_PORT PORTD
#define RX_LED_DDR  DDRD
#define RX_LED_BIT  PD6


#define UART_RX_BUFFER_SIZE 20
#define UART_TX_BUFFER_SIZE 20

void uart_init(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif
