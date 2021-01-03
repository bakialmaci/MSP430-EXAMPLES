/*
 * Pages.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include "Pages.h"
#include "ST7920.h"
#include "stdbool.h"

ST7920 ST7920;

#define number_picks_MainMenu 3
#define number_picks_Settings 2
#define number_picks_ConnectWifi 2

#define SELECT_BTN BIT4

/*
 Page Numbers;
 Main Menu: 1
 Settings:  2
 */
unsigned int PageNumber = 1;

char SSID[20] = "";
char PSWD[20] = "";

const char *picker_MainMenu[number_picks_MainMenu] =
        { "Send message", "Message box", "Settings" };

const char *picker_Settings[number_picks_Settings] = { "Connect WIFI",
                                                       "User name" };

const char *picker_ConnectWifi[number_picks_Settings] = { "Connect WIFI",
                                                          "User name" };

Pages::Pages()
{
    // TODO Auto-generated constructor stub

}

Pages::~Pages()
{
    // TODO Auto-generated destructor stub
}

void Pages::Initialize()
{
    ST7920.init();
}

void Pages::Intro()
{
    int i = 0;
    for (i = 0; i <= 8; i++)
    {
        ST7920.drawImage(LOGO, 6, i, 32, 48);
        __delay_cycles(30000);
        ST7920.fillScreen(0x00);
    }
}

void Pages::MainMenu(int opt, bool init_state)
{
    ST7920.fillScreen(0x00);
    ST7920.drawRightArrow(0, init_state ? 8 : ((opt * 10) + 8));
    for (volatile unsigned int i = 0; i < number_picks_MainMenu; i++)
    {
        ST7920.printString(picker_MainMenu[i], 1, (i * 10) + 8);
    }
}

void Pages::Settings(int opt, bool init_state)
{
    ST7920.fillScreen(0x00);
    ST7920.drawRightArrow(0, init_state ? 8 : ((opt * 10) + 8));
    for (volatile unsigned int i = 0; i < number_picks_Settings; i++)
    {
        ST7920.printString(picker_Settings[i], 1, (i * 10) + 8);
    }
}

void Pages::ConnectWifi(int opt, const char *string)
{
    opt ? strcat(PSWD, string) : strcat(SSID, string);

    ST7920.fillScreen(0x00);
    ST7920.printString("SSID:", 0, 23);
    ST7920.printString("PSWD:", 0, 33);

    ST7920.printString(PSWD, 3, 23);
    ST7920.printString(SSID, 3, 33);

}

void Pages::Typing(int opt, const char *string)
{
    switch (PageNumber)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        ConnectWifi(opt % number_picks_ConnectWifi, string);
        break;
    }

}

void Pages::Picker(int opt, bool btn_state)
{
    if (btn_state)
    {
        switch (PageNumber)
        {
        case 1: // Main Page
            switch ((opt % number_picks_MainMenu))
            {
            case 0: // Option
                break;
            case 1: // Option
                break;
            case 2: // Option
                Settings(opt % number_picks_Settings, true);
                PageNumber = 2; // Settings Page
                break;
            }
            break;
        case 2: // // Settings Page
            switch ((opt % number_picks_Settings))
            {
            case 0: // Option
                ConnectWifi(opt % number_picks_ConnectWifi, "");
                PageNumber = 3; // Settings Page
                break;
            case 1: // Option
                break;
            case 2: // Option
                break;
            case 3:
                break;
            }
            break;
        }
    }
    else
    {
        switch (PageNumber)
        {
        case 1: // Main Page
            MainMenu(opt % number_picks_MainMenu, false);
            break;
        case 2: // Settings Page
            Settings(opt % number_picks_Settings, false);
            break;
        case 3: // ConnectWifi Page
            ConnectWifi(opt % number_picks_ConnectWifi, "");
            break;
        }
    }
}

