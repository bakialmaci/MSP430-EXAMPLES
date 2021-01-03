/*
 * main.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include "msp430fr4133.h"
#include "stdlib.h"
#include "Pages.h"
#include "UTILS.h"

Pages Pages;
UTILS UTILS;

#define SELECT_BTN BIT4

int option_num = 0;

unsigned int ADC_Result = 512;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    Pages.Initialize();
    Pages.Intro();
    Pages.Picker(abs(option_num), false);

    UTILS.configure_adc();
    UTILS.init_gpio();

    while (1)
    {
        ADCCTL0 |= ADCENC | ADCSC;  // Sampling and conversion start

        if( !(P1IN & SELECT_BTN) )
        {
            Pages.Picker(abs(option_num), true);
            UTILS.delay_ms(200);
        }

        if( (BIT6 & P2IN) == 0x00 )
        {
            P1OUT ^= BIT0;
            Pages.Typing(abs(option_num), "a");
            UTILS.delay_ms(200);
        }

    }
}


// ADC interrupt service routine
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    switch (__even_in_range(ADCIV, ADCIV_ADCIFG))
    {
    case ADCIV_NONE:
        break;
    case ADCIV_ADCOVIFG:
        break;
    case ADCIV_ADCTOVIFG:
        break;
    case ADCIV_ADCHIIFG:
        break;
    case ADCIV_ADCLOIFG:
        break;
    case ADCIV_ADCINIFG:
        break;
    case ADCIV_ADCIFG:
        ADC_Result = ADCMEM0;

        if (ADC_Result < 400)
        {
            option_num++;
            Pages.Picker(abs(option_num), false);
            UTILS.delay_ms(200);
        }
        else if (ADC_Result > 1000)
        {
            option_num--;
            Pages.Picker(abs(option_num), false);
            UTILS.delay_ms(200);
        }
        break;
    default:
        break;
    }
}
