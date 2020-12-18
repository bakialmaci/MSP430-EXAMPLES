// Thanks to https://github.com/abhishekshukla94

#include <msp430.h>

#define RED_ON  0x01 // P1.0 is the red LED -> 0000 0001
#define RED_OFF 0x7E // Used to turn off the red LED -> 1111 1110
#define BUTTON  0x04 // P1.2 is the push-button -> 0000 0100

#define DEVELOPMENT 0x5A80 // Stop the watch dog timer
#define ENABLE_PINS 0xFFFE // Required to use inputs and outputs

void main(){
    WDTCTL = DEVELOPMENT; // Need for development mode
    PM5CTL0 = ENABLE_PINS; // Prepare pins for I/O usage

    P1DIR = RED_ON; // P1.0 is output, others(P1.1, P1.2, ... ,P1.7) are input.
    P1REN = BUTTON; // Input Button(p1.2) resistors activated.
    P1OUT = BUTTON; // Input Button(p1.2) used to be a pull-up resistor other pull-down.(Button pressed = 0)

    while(1){
        /*
        Not pressed -> BUTTON = 0000 0100 & P1IN = 0000 0100 --> 0000 0100 -> 0X04
        Pressed     -> BUTTON = 0000 0100 & P1IN = 1111 1011 --> 0000 0000 -> 0X00
        */
        while((BUTTON & P1IN) == 0x00){
            P1OUT = P1OUT | RED_ON; // 0000 0100 | 0000 0001 = 0000 0101 -> P1.0 ON  (P1.2 ALWAYS 1, DON'T DESTROY IT)
        }
        P1OUT = P1OUT & RED_OFF;    // 0000 0100 | 1111 1110 = 0000 0100 -> P1.0 OFF (P1.2 ALWAYS 1, DON'T DESTROY IT)
    }
}
