/*
 * File:   main.c
 * Author: lobinhojr
 *
 * Created on 26 de Outubro de 2019, 07:28
 */


#include <xc.h>

//Os "defines" abaixo são sugestões, podem ser alterados da forma que preferir
//Datasheet no link http://ww1.microchip.com/downloads/en/devicedoc/40044d.pdf

//Página 23
#define PORTB_PULLUP    OPTION_REGbits.nRBPU    //1 = disable       0 = enable
#define INT_RB0         OPTION_REGbits.INTEDG   //1 = rising edge   0 = falling edge
#define TIMER0_CS       OPTION_REGbits.T0CS     //1 = pin           0 = CLKOUT
#define TIMER0_SE       OPTION_REGbits.T0SE
#define T0_PRESC        OPTION_REGbits.PSA
#define PS_RATE         OPTION_REGbits.PS       // 0 a 7

//página 24
#define GLOBAL_INT      INTCONbits.GIE
#define PERIF_INT       INTCONbits.PEIE
#define TIMER0_INT      INTCONbits.T0IE
#define RB0_INT         INTCONbits.INTE
#define PORTB_INT       INTCONbits.RBIE
#define TIMER0_FLAG     INTCONbits.T0IF
#define RB0_FLAG        INTCONbits.INTF
#define PORTB_FLAG      INTCONbits.RBIF

//página 25
#define EEWRITE_INT     PIE1bits.EEIE
#define COMPARATOR_INT  PIE1bits.CMIE
#define RX_INT          PIE1bits.RCIE
#define TX_INT          PIE1bits.TXIE
#define CCP1_INT        PIE1bits.CCP1IE
#define TIMER2_INT      PIE1bits.TMR2IE
#define TIMER1_INT      PIE1bits.TMR1IE

//página 26
#define EEWRITE_FLAG    PIR1bits.EEIF
#define COMPARATOR_FLAG PIR1bits.CMIF
#define RX_FLAG         PIR1bits.RCIF
#define TX_FLAG         PIR1bits.TXIF
#define CCP1_FLAG       PIR1bits.CCP1IF
#define TIMER2_FLAG     PIR1bits.TMR2IF
#define TIMER1_FLAG     PIR1bits.TMR1IF

//página 27
#define OSC_FREQ        PCONbits.OSCF
#define PON_RESET       PCONbits.nPOR
#define BRO_RESET       PCONbits.nBOR

//página 31
#define RA0             PORTAbits.RA0
#define RA1             PORTAbits.RA1
#define RA2             PORTAbits.RA2
#define RA3             PORTAbits.RA3
#define RA4             PORTAbits.RA4
#define RA5             PORTAbits.RA5
#define RA6             PORTAbits.RA6
#define RA7             PORTAbits.RA7

//página 31
#define RB0             PORTBbits.RB0
#define RB1             PORTBbits.RB1
#define RB2             PORTBbits.RB2
#define RB3             PORTBbits.RB3
#define RB4             PORTBbits.RB4
#define RB5             PORTBbits.RB5
#define RB6             PORTBbits.RB6
#define RB7             PORTBbits.RB7

//página 31
#define TA0             TRISAbits.TRISA0
#define TA1             TRISAbits.TRISA1
#define TA2             TRISAbits.TRISA2
#define TA3             TRISAbits.TRISA3
#define TA4             TRISAbits.TRISA4
#define TA5             TRISAbits.TRISA5
#define TA6             TRISAbits.TRISA6
#define TA7             TRISAbits.TRISA7

//página 31
#define TB0             TRISBbits.TRISB0
#define TB1             TRISBbits.TRISB1
#define TB2             TRISBbits.TRISB2
#define TB3             TRISBbits.TRISB3
#define TB4             TRISBbits.TRISB4
#define TB5             TRISBbits.TRISB5
#define TB6             TRISBbits.TRISB6
#define TB7             TRISBbits.TRISB7

//página 48
#define T1_PRESC        T1CONbits.T1CKPS    //0 a 3
#define T1_OSC          T1CONbits.T1OSCEN
#define T1_SYNC         T1CONbits.nT1SYNC
#define T1_SW           T1CONbits.TMR1CS
#define TIMER1_ON       T1CONbits.TMR1ON
#define TIMER1          (TMR1H<<8)+(TMR1L)

//página 53
#define T2_POST         T2CONbits.TOUTPS
#define TIMER2_ON       T2CONbits.TMR2ON
#define T2_PRESC        T2CONbits.T2CKPS

//página 55
#define CCP1_MODE       CCP1CONbits.CCP1M

//página 61
#define COMPARA2        CMCONbits.C2OUT
#define COMPARA1        CMCONbits.C1OUT
#define COMP2_INV       CMCONbits.CM
#define COMP1_INV       CMCONbits.CM
#define INPUT_SW        CMCONbits.CIS
#define COMP_MODE       CMCONbits.CM        //off=0b111   on= 0b000 a 0b110

//página 67
#define VREF            VRCONbits.VREN
#define VREF_PIN        VRCONbits.VROE
#define RANGE           VRCONbits.VRR
#define VREF_VALUE      VRCONbits.VR        // 0 a 62,5% do VDD ou 25 a 71,9%

//página 71
#define CSRC            TXSTAbits.CSRC
#define TX9             TXSTAbits.TX9
#define TXEN            TXSTAbits.TXEN
#define SYNC            TXSTAbits.SYNC
#define BRGH            TXSTAbits.BRGH
#define TRMT            TXSTAbits.TRMT
#define TX9D            TXSTAbits.TX9D

//página 72
#define SPEN            RCSTAbits.SPEN
#define RX9             RCSTAbits.RX9
#define SREN            RCSTAbits.SREN
#define CREN            RCSTAbits.CREN
#define ADEN            RCSTAbits.ADEN
#define FERR            RCSTAbits.FERR
#define OERR            RCSTAbits.OERR
#define RX9D            RCSTAbits.RX9D


//Configurar Porta A - pág 35
void porta_A_config(int tris, int port){
                        //Desliga os pinos analógicos
    COMP_MODE = 7;      //desliga os comparadores
    VREF_PIN = 0;       //despluga o pin do VREF
    TRISA = tris;
    PORTA = port;
}

//Configurar Porta B - pág 43
void porta_B_config(int tris, int port){
    
    PORTB_PULLUP = 1;   //desabilita os Pullups
    TRISB = tris;
    PORTB = port;  
}

//Configrurar Timer0 - pág. 45
void timer_0_config(){
    GLOBAL_INT = 1;     //ENABLE=1      DISABLE=0
    TIMER0_INT = 1;     //ENABLE=1      DISABLE=0
    TIMER0_CS = 1;      //ENABLE=1      DISABLE=0
    TIMER0_SE = 1;      //
    T0_PRESC = 0;
    PS_RATE = 1;    
    //TIMER0_FLAG
    //TA4
}

//Configrurar Timer1 - pág. 48
void timer_1_config(){
    GLOBAL_INT = 1;     //ENABLE=1      DISABLE=0
    PERIF_INT = 1;
    TIMER1_ON = 1;
    T1_PRESC = 0;
    T1_OSC = 0;
    T1_SYNC = 0;
    T1_SW = 0;
    //TMR1L e TMR1H
}

//Configrurar Timer2 - pág. 52
void timer_2_config(){
    GLOBAL_INT = 1;     //ENABLE=1      DISABLE=0
    PERIF_INT = 1;
    TIMER2_ON = 1;
    T2_PRESC = 0;
    T2_POST = 0;
    //PR2
    //TMR2
    //TIMER2_FLAG
    //TIMER2_INT
}

//Configurar CCP1 - pág. 55 até 57
void ccp_1_config(){
    CCP1_MODE = 0;  //desliga CCP1
}

//Configurar PWM - pág. 58

//Configurar Comparador - pág. 61

//Configurar USART - pág. 71 a 88

//Configurar Memória EEPROM - pág. 89
//EEDATA    EEADR   EECON1  

//outras configda CPU - pág. 96

//configurar oscilador - pág. 97

//outras interrupções - pág. 107

//watchdog - pág. 109

//mode sleep - pág. 110

//ICSP e LVP - pág. 112

//




