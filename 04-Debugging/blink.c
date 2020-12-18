#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    volatile int almaci = 0xff;
    volatile int baki_almaci = 66;
    int i;
    for(i = 3; i >= 0; i--){
        baki_almaci = baki_almaci >> 1;
    }
    almaci = baki_almaci & 0x30;
}
