#ifndef _SI570_H_
#define _SI570_H_

#include "i2c.h"


#define SI570_ADDR 0xAA        ///< Si570 i2c address in 8bit format
#define SI570_START_FREQ 56.32 ///< Si570 Startup frequency in MHz
#define SI570_FDCO_MIN 4850    ///< Min frequency (MHz) of reference clock
#define SI570_FDCO_MAX 5670    ///< Max frequency (MHz) of reference clock

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

void si570_set_frequency(double frequency);

void si570_read_registers(void);

void si570_write_registers(void);

#endif
