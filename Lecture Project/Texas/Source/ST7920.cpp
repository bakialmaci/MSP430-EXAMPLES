/*
 * ST7920.cpp
 *
 *  Created on: Dec 23, 2020
 *
 */

#include "../Library/ST7920.h"
#include "msp430fr4133.h"
#include <string>

int Graphic_Check = 0;

ST7920::ST7920()
{
    // TODO Auto-generated constructor stub

}

ST7920::~ST7920()
{
    // TODO Auto-generated destructor stub
}

void ST7920::init()
{
    spi.init();

    sendCommand(LCD_DISPLAYOFF);
    sendCommand(LCD_CLS);
    sendCommand(LCD_ADDRINC);
    sendCommand(LCD_DISPLAYON);
    sendCommand(LCD_HOME);
    fillScreen(0x00);
}

void ST7920::GraphicMode (int enable)   // 1-enable, 0-disable
{
    if (enable == 1)
    {
        sendCommand(0x30);  // 8 bit mode
        sendCommand(0x34);  // switch to Extended instructions
        sendCommand(0x36);  // enable graphics
        Graphic_Check = 1;  // update the variable
        __delay_cycles(10000);
    }

    else if (enable == 0)
    {
        sendCommand(0x30);  // 8 bit mode
        Graphic_Check = 0;  // update the variable
        __delay_cycles(10000);
    }
}

void ST7920::sendCommand(unsigned char cmd)
{
    spi.setCS(1);
    spi.write(0xF8);
    spi.write(cmd & 0xF0);
    spi.write(cmd << 4);
    spi.setCS(0);
    __delay_cycles(1000);
}

void ST7920::sendData(unsigned char data)
{
    spi.setCS(1);
    spi.write(0xFA);
    spi.write(data & 0xF0);
    spi.write((data << 4) & 0xF0);
    spi.setCS(0);
}

void ST7920::gotoXY(int x, int y)
{
    if (y >= 32 && y < 64)
    {
        y -= 32;
        x += 8;
    }
    else if (y >= 64 && y < 96)
    {
        y -= 32;
        x += 0;
    }
    else if (y >= 96 && y < 128)
    {
        y -= 64;
        x += 8;
    }
    sendCommand(LCD_ADDR | y); // 8 -> 88
    sendCommand(LCD_ADDR | x); // 1 -> 81
}

void ST7920::fillScreen(unsigned char background)
{
    sendCommand(LCD_CLS);
    __delay_cycles(10000);
}

void ST7920::SendString(int row, int col, std::string str)
{
    switch (row)
    {
    case 0:
        col |= 0x80;
        break;
    case 1:
        col |= 0x90;
        break;
    case 2:
        col |= 0x88;
        break;
    case 3:
        col |= 0x98;
        break;
    default:
        col |= 0x80;
        break;
    }

    sendCommand(col);

    for(char& c : str) {
        sendData(c);
    }

//    while (*str)
//    {
//        sendData(*str++);
//    }
}

void ST7920::printString(std::string str1, int x, int y)
{
    for (unsigned int i = 0; i < 5; i++)
    {
        gotoXY(x, y + i);
        for (unsigned int c = 0; c < str1.length(); c++)
        {
            char index;
            if (str1[c] == ' ')
            {
                index = (('z' - '0') + 1);
            }
            else if (str1[c] == '-')
            {
                index = ('z' - '0' + 2);
            }
            else
            {
                index = (str1[c] - '0');
            }
            sendData(FONT[(index * 5 + i)]);
        }
    }
}

void ST7920::drawImage(const char *image, int x, int y, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        gotoXY(x / 2, y + i);

        if (x % 2 == 1)
        {
            sendData(0x00);
        }
        for (int j = 0; j < width / 8; j++)
        {
            sendData(image[i * (width / 8) + j]);
        }
    }
}

void ST7920::drawRightArrow(int x, int y)
{
    GraphicMode(1);
    for (int i = 0; i < 8; i++)
    {
        gotoXY(x, i + y);
        sendData(RIGHT_ARROW[i] >> 8);
        sendData(RIGHT_ARROW[i]);
    }
    //GraphicMode(0);
}
