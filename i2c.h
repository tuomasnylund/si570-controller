/*
 * I2C TEST LIBRARY
 * -Tuomas Nylund
 */
#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define TWBR_VAL 16
//#define TWBR_VAL 32

void i2c_init(void);

void i2c_send_start(void);

void i2c_send_stop(void);

void i2c_send_addr(uint8_t addr);

void i2c_send_byte(uint8_t data);

uint8_t i2c_read_byte(void);


void i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr);

#endif
