#include <18F2550.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)

#use delay(crystal=20000000)

#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=lcm_serial)

#use FIXED_IO( A_outputs=PIN_A4,PIN_A3,PIN_A2,PIN_A1 )
#define analog_A0   PIN_A0
#define digital_output_A1   PIN_A1
#define digital_output_A2   PIN_A2
#define digital_output_A3   PIN_A3
#define digital_output_A4   PIN_A4

#define LOW 0
#define HIGH 1

