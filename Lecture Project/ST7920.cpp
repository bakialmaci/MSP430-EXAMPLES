/*
 * ST7920.cpp
 *
 *  Created on: Dec 23, 2020
 *
 */

#include "ST7920.h"
#include "msp430fr4133.h"


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
    __delay_cycles(50000);
    sendCommand(LCD_ADDRINC);
    sendCommand(LCD_DISPLAYON);
    sendCommand(LCD_HOME);

    sendCommand(LCD_BASIC);
    sendCommand(LCD_EXTEND);
    sendCommand(LCD_GFXMODE);

    fillScreen(0x00);
    __delay_cycles(50000);
}

void ST7920::sendCommand(unsigned char cmd)
{
    spi.setCS(1);
    spi.write(0xF8);
    spi.write(cmd & 0xF0);
    spi.write(cmd << 4);
    spi.setCS(0);
}

void ST7920::sendData(unsigned char data)
{
    spi.setCS(1);
    spi.write(0xFA);
    spi.write(data & 0xF0);
    spi.write(data << 4);
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
    sendCommand(LCD_ADDR | y); // 6-bit (0..63)
    sendCommand(LCD_ADDR | x); // 4-bit (0..15)
}

void ST7920::fillScreen(unsigned char background)
{
    for (unsigned int j = 0; j < 32; j++)
    {
        gotoXY(0, j);
        spi.setCS(1);
        spi.write(0xFA);
        for (unsigned int i = 0; i < 16; i++)
        {
            spi.write(background & 0xF0);
            spi.write(background << 4);
        }

        for (unsigned int i = 0; i < 16; i++)
        {
            spi.write(background & 0xF0);
            spi.write(background << 4);
        }
        spi.setCS(0);
        void goToPage(unsigned char page);
    }
}

void ST7920::fillRectangle(unsigned char background, int x, int y, int width,
                           int height)
{
    for (unsigned int j = 0; j < height; j++)
    {
        gotoXY(x / 2, y + j);
        spi.setCS(1);
        spi.write(0xFA);

        if (x % 2 == 1)
        {
            spi.write(0);
            spi.write(0);
        }
        for (unsigned int i = 0; i < width; i++)
        {
            spi.write(background & 0xF0);
            spi.write(background << 4);
        }

        spi.setCS(0);
    }
}

void ST7920::drawMultipleVerticalLines(unsigned char times, int x, int y,
                                       int height)
{

    for (unsigned int j = 0; j < height; j++)
    {
        gotoXY(x / 2, y + j);

        spi.setCS(1);
        spi.write(0xFA);

        if (x % 2)
        {
            spi.write(0);
            spi.write(0);
        }
        unsigned char line = 0;
        for (unsigned char i = 0; i < times; i++)
        {

            line += 2 << ((3 - (i % 4)) * 2);
            if ((i + 1) % 4 == 0 || i == times - 1)
            {
                spi.write(line & 0xF0);
                spi.write(line << 4);
                line = 0;
            }
        }
        spi.setCS(0);
    }
}

void ST7920::printString(const char *string, int x, int y)
{
    for (unsigned int i = 0; i < 8; i++)
    {
        gotoXY(x, y + i);
        for (unsigned int c = 0; c < strlen(string); c++)
        {
            char index;
            if (string[c] == ' ')
            {
                index = (('z' - '0') + 1);
            }
            else if (string[c] == '-')
            {
                index = ('z' - '0' + 2);
            }
            else
            {
                index = (string[c] - '0');
            }
            sendData(FONT[index * 8 + i]);
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

    for (int i = 0; i < 8; i++)
    {
        gotoXY(x, i + y);
        sendData(RIGHT_ARROW[i] >> 8);
        sendData(RIGHT_ARROW[i]);
    }
}
