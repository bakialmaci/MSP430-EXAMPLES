#include <msp430.h>

#define RED_ON  0x01 // P1.0 is the red LED -> 0000 0001
#define RED_OFF 0x7E // Used to turn off the red LED -> 1111 1110
#define BUTTON  0x04 // P1.2 is the push-button -> 0000 0100

#define DEVELOPMENT 0x5A80 // Stop the watch dog timer
#define ENABLE_PINS 0xFFFE // Required to use inputs and outputs

void delay_ms(unsigned int ms){
    while (ms){
        __delay_cycles(1000);
        ms--;
    }
}

void main(){
    WDTCTL = DEVELOPMENT; // Need for development mode
    PM5CTL0 = ENABLE_PINS; // Prepare pins for I/O usage

    P1DIR = RED_ON; // P1.0 is output, others(P1.1, P1.2, ... ,P1.7) are input.
    P1REN = BUTTON; // Input Button(p1.2) resistors activated.
    P1OUT = BUTTON; // Input Button(p1.2) used to be a pull-up resistor other pull-down.(Button pressed = 0)

    volatile int counter = 0;


    while(1){
        if((BUTTON & P1IN) == 0x00){
            while((BUTTON & P1IN) == 0x00);
            counter++;
        }

        if(counter & 0x1) P1OUT = P1OUT | RED_ON;
        else P1OUT = P1OUT & RED_OFF;

        delay_ms(50); // to debouncing
    }
}
