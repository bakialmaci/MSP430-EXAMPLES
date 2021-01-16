/*
 * UTILS.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include "../Library/UTILS.h"
#include "msp430fr4133.h"
#include "Library/Pages.h"
#include "stdlib.h"
#include <string>

#define SELECT_BTN BIT4

extern Pages Pages;
extern UTILS UTILS;

int option_num = 0;
volatile unsigned int val = 0;
unsigned int ADC_Result = 512; //initialized.

void UTILS::init_gpio(){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    /* Start Keyboard Clock Pin */
    P1REN |= BIT7; // configure P1.2 as input Switch connection
    P1IE |= BIT7; // enable P1.2 interrupt
    P1IFG &= ~BIT7; // clear the interrupt flag
    /* End Keyboard Clock Pin */

    /* Start Keyboard Data Pin */
    P1DIR &= ~BIT5;
    P1REN = BIT5;
    P1OUT = BIT5;
    /* End Keyboard Data Pin */

    /* Start LCD Init */
    P1DIR &= ~SELECT_BTN;
    P1REN = SELECT_BTN;
    P1OUT = SELECT_BTN;
    /* End LCD Init */

    __bis_SR_register(GIE);

}

void UTILS::delay_ms(unsigned int ms)
{
    while (ms)
    {
        __delay_cycles(1000);
        ms--;
    }
}

void UTILS::checkADC()
{
    /* Start Read Joystick Values */
    #if defined (__MSP430FR4133__)
    ADCCTL0 |= ADCENC | ADCSC;  // Sampling and conversion start
    if (!(P1IN & BIT4))         // Joystick button state. (Will be deprecated)
        Pages.Picker(option_num, true);
    #endif
    /* End Read Joystick Values */
}


/* Start ADC Configuration */
void UTILS::configure_adc()
{
    SYSCFG2 |= ADCPCTL1;
    PM5CTL0 &= ~LOCKLPM5;
    ADCCTL0 |= ADCSHT_2 | ADCON;       // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                 // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                 // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_3;             // A1 ADC input select; Vref=AVCC
    ADCIE |= ADCIE0;                   // Enable ADC conv complete interrupt
}
/* End ADC Configuration */


/* Start ADC Handler Interrput */
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    ADC_Result = ADCMEM0;
    if (ADC_Result < 400)
        Pages.Picker((--option_num), false);
    else if (ADC_Result > 1000)
        Pages.Picker((++option_num), false);

}
/* End ADC Handler Interrput */


/* Start Keyboard Handler Interrput */
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_IntHandler(void)
{
    val = 0;
    for (volatile unsigned int i = 0; i < 11; i++)
    {
        while ((BIT7 & P1IN) == 0x01);
        val |= (BIT5 & P1IN) << i;
        while ((BIT7 & P1IN) == 0x00);
    }
    if (!strcmp(UTILS.toASCII(val), "up"))
        Pages.Picker((--option_num), false);
    else if (!strcmp(UTILS.toASCII(val), "down"))
        Pages.Picker((++option_num), false);
    else if (!strcmp(UTILS.toASCII(val), "esc"))
        Pages.Typing(0, "esc");
    else if (!strcmp(UTILS.toASCII(val), "\n")){
        Pages.Picker(option_num, true);
        option_num= 0;
    }
    else
        Pages.Typing(option_num, UTILS.toASCII(val));

    P1IFG &= ~BIT7;
}
/* End Keyboard Handler Interrput */


const char *UTILS::toASCII(unsigned int keyVal){
    switch (keyVal)
    {
    case 50048:
        return "a";
    case 50752:
        return "b";
    case 58400:
        return "c";
    case 50272:
        return "d";
    case 58496:
        return "e";
    case 58720:
        return "f";
    case 50816:
        return "g";
    case 58976:
        return "h";
    case 51776:
        return "i";
    case 51040:
        return "j";
    case 59456:
        return "k";
    case 59744:
        return "l";
//    case 59200:
//        return "m";
    case 50720:
        return "n";
    case 59520:
        return "o";
    case 59808:
        return "p";
    case 58784:
        return "r";
    case 58208:
        return "s";
    case 50560:
        return "t";
    case 59264:
        return "u";
    case 50496:
        return "v";
    case 59040:
        return "y";
    case 49984:
        return "z";
    case 58272:
        return "w";
    case 58432:
        return "x";
    case 49824:
        return "q";
    case 60608:
        return "bck";
    case 59200:
        return "esc";
    case 52896:
        return "up";
    case 52576:
        return "left";
    case 60992:
        return "down";
    case 61056:
        return "right";
    case 50464:
        return " ";
    case 60224:
        return "\n";
    case 52736:
        return "0";
    case 63104:
        return "1";
//    case 60992:
//        return "2";
    case 53056:
        return "3";
//    case 52576:
//        return "4";
    case 59168:
        return "5";
    case 63296:
        return "6";
    case 60800:
        return "7";
//    case 59200:
//        return "8";
    case 63424:
        return "9";
    }

    return "";
}
