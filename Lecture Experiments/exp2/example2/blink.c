#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watch dog timer
    PM5CTL0 &= ~LOCKLPM5;   //previous port settings are activated.

    P1DIR |= BIT0; // P1.0 is configured as output
    P1OUT = ~BIT0; // Led on P1.0 is turned off

    TA1CCR0 = 32797;
    TA1CTL = TASSEL_1 | MC_1 | TACLR;

    while (1)
    {
        if (TA1CTL & TAIFG == TAIFG)
        {
            P1OUT ^= BIT0;
            TA1CTL &= ~TAIFG;
        }
    }

}
