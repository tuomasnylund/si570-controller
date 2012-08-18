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
#ifndef _SI570_H_
#define _SI570_H_

#include "i2c.h"


#define SI570_ADDR 0xAA ///< Si570 i2c address in 8bit format

#define SI570_START_FREQ 56.32 ///< Si570 Startup frequency in MHz

#define SI570_FDCO_MAX 5670
#define SI570_FDCO_MIN 4850

/* SI570 registers:
 * 7:  HS_DIV[2:0], N1[6:2]
 * 8:  N1[1:0], RFREQ[37:32]
 * 9:  RFREQ[31:24]
 * 10: RFREQ[23:16]
 * 11: RFREQ[15:8]
 * 12: RFREQ[7:0]
 * 13-18: registers 7-11 repeated for 7ppm version
 * 135: RST_REG,NewFreq,Freeze M,Freeze VCADC,0,0,0,RECALL
 * 137: 0,0,0,FREEZE_DCO,0,0,0,0
 */


inline void si570_reset(void){
    //recall NVM bits into RAM
    i2c_write(SI570_ADDR,135,0x01);
}

void si570_init(void);

double si570_get_frequency(void);
void   si570_set_frequency(double frequency);

void si570_read_registers(void);
void si570_write_registers(void);

uint8_t si570_get_hsdiv(void);
void    si570_set_hsdiv(uint8_t hsdiv);

uint8_t si570_get_n1(void);
void    si570_set_n1(uint8_t n1);

uint32_t si570_get_rfreq_truncated(void);
void     si570_set_rfreq_truncated(uint32_t rfreq);

#endif
