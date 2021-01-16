/*
 * main.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include <msp430.h>
#include "Library/Pages.h"
#include "Library/UTILS.h"

Pages Pages;
UTILS UTILS;

void main(void)
{
    /* Start Initializations */
    UTILS.init_gpio();
    UTILS.configure_adc();
    Pages.Initialize();
    Pages.Intro();
    /* End Initializations */

    while (true)
    {
        UTILS.checkADC();
    }
}
