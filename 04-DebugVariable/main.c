#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;// Disable the GPIO power-on default highimpedance mode

     volatile int almaci = 0xff;
     volatile int baki = 66;
     int i;

     for(i = 3; i >= 0; i--){
         baki = baki << 1;
     }

     almaci = baki & 0x30;
}
