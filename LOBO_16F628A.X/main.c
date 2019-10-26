/*
 * File:   main.c
 * Author: lobinhojr
 *
 * Created on 26 de Outubro de 2019, 07:28
 */


#include <xc.h>
#include "16F6_27_28_48_A.h"


void main(void) {
    
    porta_A_config(0b00000000,0b00000000);
    porta_B_config(0b00000000,0b00000000);
    timer_0_config();
    timer_0_config();
    timer_0_config();
    
    
    
    return;
}
