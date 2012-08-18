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
#define F_CLOCK  8000000
#define F_CPU    8000000

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "si570.h" 
#include "i2c.h" 

uint8_t  si570_registers[6];      ///<Si570 register values
double   si570_fxtal;             ///<Si570 XTAL frequency
uint8_t  si570_n1_initial;        ///<N1 initial value
uint8_t  si570_hs_div_initial;    ///<HS_DIV initial value
uint32_t si570_rfreq_long_inital; ///<RFREQ initial truncated 32bit value

void si570_init(void){
    double rfreq;

    si570_reset();
    si570_read_registers();

    si570_hs_div_initial    = si570_get_hsdiv();
    si570_n1_initial        = si570_get_n1();
    si570_rfreq_long_inital = si570_get_rfreq_truncated();

    rfreq = 
        ((si570_registers[1] & 0x3F)<<4)|
        ((si570_registers[2] & 0xF0)>>4);

    si570_fxtal = ((double)SI570_START_FREQ *
            si570_hs_div_initial * si570_n1_initial) /
            rfreq;
}

double si570_get_frequency(void){
    return -1;
}

void si570_set_frequency(double frequency){
    uint8_t hs_div_values[6] = {11,9,7,6,5,4};

    uint8_t i;

    uint16_t div;
    uint16_t div_min;
    uint16_t div_max;
    uint8_t valid_combo;

    uint8_t hs_div;
    uint8_t n1;
    double  n1_float;

    double ratio;
    uint32_t rfreq_final;

    //get minimum and maximum clock dividers that work for new frequency
    div_min = ceil (SI570_FDCO_MIN/frequency);
    div_max = floor(SI570_FDCO_MAX/frequency);

    //go through the dividers and find one that is 
    //possible to make with HS_DIV and N1 combo
    valid_combo = 0;
    for (div = div_min ; div <= div_max ; div++){
        for (i=0 ; i<6 ; i++){
            hs_div = hs_div_values[i]; //get a valid HS_DIV value
            n1_float = (double)div/(double)hs_div; //calculate N1 value

            if ((n1_float - floor(n1_float)) == 0){ // is n1_float an integer?
                n1 = (uint8_t) n1_float;
                if ((n1 == 1) || ((n1&1)==0)){ //is it a valid N1 divider?
                    valid_combo = 1; //Found good dividers! Let's GTFO
                }

            }
            if (valid_combo)
                break;
        }
        if (valid_combo)
            break;
    }
    if (valid_combo == 0) //We failed at finding valid dividers :(
        return;

    ratio  = frequency / SI570_START_FREQ;
    ratio *= (double)n1    /(double)si570_n1_initial;
    ratio *= (double)hs_div/(double)si570_hs_div_initial;

    rfreq_final = ratio * si570_rfreq_long_inital;

    //set new values to the registers array
    si570_set_rfreq_truncated(rfreq_final);
    si570_set_n1(n1);
    si570_set_hsdiv(hs_div);
}

void si570_write_registers(void){
    uint8_t i;
    uint8_t tmp_reg135;
    uint8_t tmp_reg137;

    //get current values of registers 
    tmp_reg135 = i2c_read(SI570_ADDR,135);
    tmp_reg137 = i2c_read(SI570_ADDR,137);

    //Freeze DCO
    tmp_reg137 |= (1<<4);
    i2c_write(SI570_ADDR,137,tmp_reg137);

    //write new configuration
    for ( i=0 ; i<6 ; i++)
        i2c_write(SI570_ADDR,7+i,si570_registers[i]);

    //Unfreeze DCO
    tmp_reg137 &= ~(1<<4);
    i2c_write(SI570_ADDR,137,tmp_reg137);
    
    //Set NewFreq bit
    tmp_reg135 |= (1<<6);
    i2c_write(SI570_ADDR,135,tmp_reg135);

}

void si570_read_registers(void){
    uint8_t i;
    for ( i=0 ; i<6 ; i++)
        si570_registers[i] = i2c_read(SI570_ADDR,7+i);
}

uint8_t si570_get_hsdiv(void){
    return (si570_registers[0]>>5) + 4;
}

void si570_set_hsdiv(uint8_t hsdiv){

    //if (hsdiv == 8 || hsdiv == 10)
    //    return;

    hsdiv -= 4;

    si570_registers[0] &= 0b00011111;
    si570_registers[0] |= hsdiv<<5;
}

uint8_t si570_get_n1(void){
    uint8_t n1;
    n1  = (si570_registers[0] & 0b00011111)<<2;
    n1 |= (si570_registers[1]>>6) & 0b11;

    if (n1 == 0)
        n1 = 1;
    else if ((n1&1) != 0)
        n1 += 1;

    return n1;
}

void si570_set_n1(uint8_t n1){

    if (n1 == 1)
        n1 = 0;
    else if ((n1&1) == 0)
        n1 -= 1;

    si570_registers[0] &= 0b11100000;
    si570_registers[1] &= 0b00111111;

    si570_registers[0] |= ((n1>>2) & 0b00011111);
    si570_registers[1] |= (n1<<6) & 0b11000000;
}

uint32_t si570_get_rfreq_truncated(void){
    uint32_t retval;

    retval = si570_registers[1] & 0x3F;
    retval = (retval<<8) +  (si570_registers[2]);
    retval = (retval<<8) +  (si570_registers[3]);
    retval = (retval<<8) +  (si570_registers[4]);
    retval = (retval<<6) + ((si570_registers[5])>>2);

    return retval;
}

void si570_set_rfreq_truncated(uint32_t rfreq){
    si570_registers[1] &=  0b11000000;

    si570_registers[1] |= rfreq>>30;
    si570_registers[2]  = rfreq>>22;
    si570_registers[3]  = rfreq>>14;
    si570_registers[4]  = rfreq>>6;
    si570_registers[5]  = rfreq<<2;
}
