/*
 * UTILS.h
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#ifndef UTILS_H_
#define UTILS_H_

class UTILS
{
public:
    void init_gpio();
    void delay_ms(unsigned int ms);
    void configure_adc();
    const char *toASCII(unsigned int keyVal);
    void checkADC();
};

#endif /* UTILS_H_ */
