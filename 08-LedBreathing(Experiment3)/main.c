#include <msp430.h> 

#define LED_MAX 100
#define LED_MIN 1
#define UPDATE_INTERVAL 400
// We can choose for which led or leds to turn on the breathing effect
#define LEDS BIT0|BIT6
//#define LEDS BIT0
//#define LEDS BIT6

unsigned int onTime = LED_MIN;
unsigned int offTime = LED_MAX;
unsigned int ledState = 1; // We start as LED turned on
unsigned int increase = 1;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= LEDS;
    P1OUT |= LEDS;
    // P1OUT ^= BIT0; // Uncomment this to see a nice effect, leds breath inverse to each other.

    TA0CTL = TASSEL_1 + ID_0 + MC_1 + TACLR;
    TA0CCR0 = LED_MIN;
    TA0CCTL0 |= CCIE;

    TA1CTL = TASSEL_1 + ID_0 + MC_1 + TACLR;
    TA1CCR0 = UPDATE_INTERVAL;
    TA1CCTL0 |= CCIE;

    __bis_SR_register(LPM3_bits + GIE); // Sleep during delay
    return 0;
}

// With this timer Led is ON for "onTime" cycles, Led is OFF for "offTime" cycles
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0a0(void)
{
    if (ledState == 0)
    {
        TA0CCR0 = onTime; // If previous state is "0" => LED was off, load the onTime to counter
    }
    else
    {
        TA0CCR0 = offTime; // If previous state is "1" => LED was on, load the offTime to counter
    }
    P1OUT ^= LEDS;
    ledState ^= 1;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1a0(void)
{
    if (increase == 1)
    {
        onTime += 1; // In this condition, increase the time LED is ON => LED gets brighter
        offTime -= 1; // Decrease the time LED is off
    }
    else{
        onTime -= 1; // In this condition, decrease the time LED is ON => LED gets dimmer
        offTime += 1; // Increase the time LED is OFF
    }
    if(onTime == LED_MAX || onTime == LED_MIN) // If we hit min or max, change direction
        increase ^= 1; // Change (increase -> decrease) or (decrease -> increase)
}
