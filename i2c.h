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
