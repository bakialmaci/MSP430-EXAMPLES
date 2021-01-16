#include <msp430.h>

void main(void)
{

    unsigned const int short_blink = 5000;
    unsigned const int long_blink = 20000;
    unsigned const int halfsecond = 30000;

    unsigned const int delays[] = { short_blink, halfsecond, halfsecond,
                                       short_blink, halfsecond, long_blink,
                                       halfsecond, short_blink, halfsecond,
                                       short_blink, halfsecond, halfsecond,
                                       short_blink, halfsecond, halfsecond,
                                       short_blink, halfsecond, short_blink,
                                       halfsecond, short_blink, halfsecond,
                                       short_blink, halfsecond, long_blink,
                                       halfsecond, halfsecond, short_blink,
                                       halfsecond, long_blink, halfsecond,
                                       long_blink, halfsecond, long_blink,
                                       halfsecond, long_blink, halfsecond,
                                       halfsecond, long_blink, halfsecond,
                                       long_blink, halfsecond, short_blink,
                                       halfsecond, short_blink, halfsecond,
                                       short_blink };
    volatile int counter = 0;

    WDTCTL = WDTPW | WDTHOLD; // stop watch dog timer
    PM5CTL0 &= ~LOCKLPM5; //previous port settings are activated.
    P1DIR |= BIT0; // P1.0 is configured as output
    P1OUT = ~BIT0; // Led on P1.0 is turned off
    TA1CCR0 = delays[0];
    TA1CTL = TASSEL_1 | MC_1 | TACLR;
    while (1)
    {
        if (TA1CTL & TAIFG == TAIFG)
        {

            if (delays[counter] == halfsecond)
            {
                P1OUT &= ~BIT0;
            }
            else
            {
                P1OUT ^= BIT0;
            }
            TA1CCR0 = delays[counter];

            TA1CTL &= ~TAIFG;

            if (counter <= 45)
            {
                counter++;
            }
            else
            {
                counter = 0;
            }

        }
    }
}
