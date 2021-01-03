/*
 * UTILS.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include <UTILS.h>
#include "msp430fr4133.h"

#define SELECT_BTN BIT4

UTILS::UTILS()
{
    // TODO Auto-generated constructor stub

}

UTILS::~UTILS()
{
    // TODO Auto-generated destructor stub
}

void UTILS::init_gpio(){
    P1DIR |= BIT0; // P1.0 output
    P1OUT &= ~BIT0; // turn off the LED

    P2REN |= BIT6; // Input Button(p1.2) resistors activated.


    P1DIR &= ~SELECT_BTN;
    P1REN = SELECT_BTN;
    P1OUT = SELECT_BTN;

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

void UTILS::configure_adc()
{
    SYSCFG2 |= ADCPCTL1;
    PM5CTL0 &= ~LOCKLPM5;
    ADCCTL0 |= ADCSHT_2 | ADCON;                       // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP;                        // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                              // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_3;                     // A1 ADC input select; Vref=AVCC
    ADCIE |= ADCIE0;                       // Enable ADC conv complete interrupt
}
