#include <msp430.h>

#define RED_ON  0x01 // P1.0 is the red LED -> 0000 0001
#define RED_OFF 0x7E // Used to turn off the red LED -> 1111 1110

#define GREEN_ON  0x01 // P1.0 is the red LED -> 0000 0001
#define GREEN_OFF 0x7E // Used to turn off the red LED -> 1111 1110

#define BUTTON  0x04 // P1.2 is the push-button -> 0000 0100

#define DEVELOPMENT 0x5A80 // Stop the watch dog timer
#define ENABLE_PINS 0xFFFE // Required to use inputs and outputs

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
    WDTCTL = DEVELOPMENT; // Need for development mode
    PM5CTL0 = ENABLE_PINS; // Prepare pins for I/O usage

    P1DIR = RED_ON; // P1.0 is output, others(P1.1, P1.2, ... ,P1.7) are input.
    P1REN = BUTTON; // Input Button(p1.2) resistors activated.
    P1OUT = BUTTON; // Input Button(p1.2) used to be a pull-up resistor other pull-down.(Button pressed = 0)

    P4DIR = GREEN_ON;
    P4OUT = BUTTON;

    int status = 0;

    while(1){
        /*
        Not pressed -> BUTTON = 0000 0100 & P1IN = 0000 0100 --> 0000 0100 -> 0X04
        Pressed     -> BUTTON = 0000 0100 & P1IN = 1111 1011 --> 0000 0000 -> 0X00
        */


        if((BUTTON & P1IN) == 0X00){
            status = (status == 3) ? 0 : status+1;
            delay_ms(100);
        }

        if((status % 4) == 0){
            P1OUT = P1OUT & RED_OFF;
            P4OUT = 0X00;
        }else if((status % 4) == 1){
            P1OUT = P1OUT | RED_ON;
            P4OUT = 0x00;
        }else if((status % 4) == 2){
            P1OUT = P1OUT & RED_OFF;
            P4OUT = 0x01;
        }else{
            P1OUT = P1OUT | RED_ON; // 0000 0100 | 0000 0001 = 0000 0101 -> P1.0 ON  (P1.2 ALWAYS 1, DON'T DESTROY IT)
            P4OUT = 0x01;
        }
    }
}
