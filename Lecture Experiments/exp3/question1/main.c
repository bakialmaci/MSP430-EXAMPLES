#include <msp430.h>
void main(void)
{
    WDTCTL = WDT_ADLY_1000; //WDT enabled to 1sec.
    PM5CTL0 &= ~LOCKLPM5; //previous port settings are activated.
    __bis_SR_register(GIE); //general interrupts enabled.
    P1DIR |= BIT0; // P1.0 is configured as output
    P1OUT = ~BIT0; // Led on P1.0 is turned off
    while (1)
    {
        if (SFRIFG1 & BIT0 == WDTIFG) // check interrupt
        {
            P1OUT ^= BIT0; // toggle led pin.
            SFRIFG1 &= ~WDTIFG; // Clear interrupt flag to next time.
        }
    }
}
