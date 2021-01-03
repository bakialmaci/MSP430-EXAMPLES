/*
 * SPI.cpp
 *
 *  Created on: Dec 23, 2020
 *
 */

#include "SPI.h"

SPI::SPI()
{
    // TODO Auto-generated constructor stub

}

SPI::~SPI()
{
    // TODO Auto-generated destructor stub
}

void SPI::init()
{
    P1DIR |= CS;
    P1DIR |= CS;

    P5SEL0 |= SCLK | MOSI;     // set 2-SPI pin as second function

    UCB0CTLW0 |= UCCKPH + UCMSB + UCMST + UCSYNC;

    UCB0CTLW0 |= UCSWRST;                     // **Put state machine in reset**

    UCB0CTLW0 |= UCMST | UCSYNC | UCCKPH | UCMSB | UCSSEL__SMCLK;

    UCB0BRW = 0x6; //0x6

    UCB0CTLW0 &= ~UCSWRST;                  // **Initialize USCI state machine**

    setCS(false);

    //__bis_SR_register(GIE);
}

void SPI::setCS(bool high)
{
    if (high)
        P1OUT |= CS;
    else
        P1OUT &= ~CS;
}

void SPI::write(unsigned char data)
{
    while (!(UCB0IFG & UCTXIFG))
        ;

    UCB0TXBUF = data;

    while (!(UCB0IFG & UCTXIFG))
        ;
}
