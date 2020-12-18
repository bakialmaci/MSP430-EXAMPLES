#include <msp430.h> 

#define DEVELOPMENT 0x5A80 // *Stop the watch dog timer
#define ENABLE_PINS 0xFFFE // *Required to use inputs and outputs

#define RED_LED  0x01 // P1.0 is the red LED -> 0000 0001

#define LONG_DELAY 1000
#define SHORT_DELAY 200

void delay_ms(unsigned int ms){
    while (ms){
        /*
         By default 1MHz clock cycle and it can be configure but not necessary for now.
         Also _delay_cycles not recommended. Timer module has more accuracy than __delay_cycles.
         */
        __delay_cycles(1000);
        ms--;
    }
}

void main(){
    WDTCTL = DEVELOPMENT;  // *Need for development mode (Required, see documentation)
    PM5CTL0 = ENABLE_PINS; // *Prepare pins for I/O usage (Required, see documentation)

    P1DIR = RED_LED; // P1.0 is output, others(P1.1, P1.2, ... ,P1.7) are input.
    P1OUT = 0x00;   // 0000 0000 -> LED Off State.

    while(1){
        // XOR gates provide to change state of red led each turns.
        P1OUT = P1OUT ^ RED_LED;
        delay_ms(SHORT_DELAY);
        P1OUT = P1OUT ^ RED_LED;
        delay_ms(SHORT_DELAY);

        P1OUT = P1OUT ^ RED_LED;
        delay_ms(LONG_DELAY);
        P1OUT = P1OUT ^ RED_LED;
        delay_ms(LONG_DELAY);
    }
}
