#include <msp430.h>
#include <stdbool.h>

#define LONG_DELAY 40000
#define SHORT_DELAY LONG_DELAY/4
#define SHORT 0
#define LONG 1

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watch dog timer
    PM5CTL0 &= ~LOCKLPM5;   //previous port settings are activated.

    P1DIR |= BIT0; // P1.0 is configured as output
    P1OUT = ~BIT0; // Led on P1.0 is turned off

    TA1CCR0 = LONG_DELAY;
    TA1CTL = TASSEL_1 | MC_1 | TACLR;

    volatile bool delay_mode = LONG;
    volatile unsigned int counter = 0;

    while (1)
    {
        if (TA1CTL & TAIFG == TAIFG) // toggle when interrupt flag is set
        {
            P1OUT ^= BIT0;
            TA1CTL &= ~TAIFG;
            counter++;
        }

        if (counter == 2)
        {
            counter = 0;
            delay_mode ^= true;
        }

        TA1CCR0 = delay_mode ? LONG_DELAY : SHORT_DELAY;
    }

}
