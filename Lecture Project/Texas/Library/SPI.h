/*
 * SPI.h
 *
 *  Created on: Dec 23, 2020
 *
 */

#include "msp430fr4133.h"

#ifndef SPI_H_
#define SPI_H_

#define SCLK    BIT1 // SPI clock at P5.1
#define MOSI    BIT2 // SPI MOSI (Master Out, Slave In) at P5.2

#define CS      BIT6 // CS (Chip Select) at P1.6

class SPI
{
public:
    void init();

    void setCS(bool high);
    void write(unsigned char data);
};

#endif /* SPI_H_ */
