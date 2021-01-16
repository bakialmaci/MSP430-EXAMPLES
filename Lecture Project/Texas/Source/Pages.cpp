/*
 * Pages.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#include "../Library/Pages.h"
#include "../Library/ST7920.h"
#include "Library/UTILS.h"
#include "stdbool.h"
#include <string>
#include "stdlib.h"

ST7920 ST7920;
extern UTILS UTILS;

#define number_picks_MainMenu 3
#define number_picks_Settings 2
#define number_picks_ConnectWifi 2

#define SELECT_BTN BIT4

unsigned int PageNumber = 1;
unsigned int rowNumber = 0;
extern int option_num;

std::string SSID = "";
std::string PSWD = "";
std::string TEXT = "";


const char *picker_MainMenu[number_picks_MainMenu] =
        { "Send message", "Message box", "Settings" };

const char *picker_Settings[number_picks_Settings] = { "Connect WIFI",
                                                       "User name" };

const char *picker_ConnectWifi[number_picks_Settings] = { "Connect WIFI",
                                                          "User name" };

void Pages::Initialize()
{
    ST7920.init();
}

void Pages::Intro()
{
    ST7920.GraphicMode(1);
    int i = 0;
    for (i = 0; i <= 8; i++)
    {
        ST7920.drawImage(LOGO, 6, i, 32, 48);
        __delay_cycles(30000);
        ST7920.fillScreen(0x00);
    }
    ST7920.GraphicMode(0);
    Picker(0, false);
}

void Pages::MainMenu(int opt, bool init_state)
{
    opt = opt % number_picks_MainMenu;
    ST7920.fillScreen(0x00);
    ST7920.SendString(init_state ? 0 : opt, 0, "->");
    for (volatile unsigned int i = 0; i < number_picks_MainMenu; i++)
    {
        ST7920.SendString(i, 1, picker_MainMenu[i]);
    }
}

void Pages::Settings(int opt, bool init_state)
{
    opt = opt % number_picks_Settings;
    ST7920.fillScreen(0x00);
    ST7920.SendString(init_state ? 0 : opt, 0, "->");
    for (volatile unsigned int i = 0; i < number_picks_Settings; i++)
    {
        ST7920.SendString(i, 1, picker_Settings[i]);
    }
}

void Pages::ConnectWifi(int opt, const char *string)
{
    opt = opt % number_picks_ConnectWifi;
    ST7920.fillScreen(0x00);

    ST7920.SendString(1, 0, "SSID:");
    ST7920.SendString(2, 0, "PSWD:");

    if(opt == 0){
        if(!strcmp(string, "bck") && SSID.length()){
            SSID.pop_back();
        }else if(strcmp(string, "bck") && SSID.length() <= 8){
            SSID += string;
        }
    }else{
        if(!strcmp(string, "bck") && PSWD.length()){
            PSWD.pop_back();
        }else if(strcmp(string, "bck") && PSWD.length() <= 8){
            PSWD += string;
        }
    }

    ST7920.SendString(1, 3, SSID);
    ST7920.SendString(2, 3, PSWD);
}

void Pages::Typing(unsigned int opt, char const * string)
{
    if(!strcmp(string, "esc")){
        PageNumber = 1;
        Picker(0, false);
    }else{
        switch (PageNumber)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            ConnectWifi(opt, string);
            break;
        case 4:
            SendMessage(string);
            break;
        }
    }


}


void Pages::SendMessage(const char *string)
{
    ST7920.fillScreen(0x00);

    if(!strcmp(string, "bck") && TEXT.length()){
        TEXT.pop_back();
    }else if(strcmp(string, "bck") && TEXT.length() <= 12){
        TEXT += string;
    }

    ST7920.SendString(0, 0, TEXT);

}


void Pages::Picker(unsigned int opt, bool btn_state)
{
    if (btn_state)
    {
        switch (PageNumber)
        {
        case 1: // Main Page
            switch ((opt % number_picks_MainMenu))
            {
            case 0: // Option
                SendMessage("");
                PageNumber = 4; // Send Message Pages
                option_num = 0;
                break;
            case 1: // Option
                break;
            case 2: // Option
                Settings(opt % number_picks_Settings, true);
                PageNumber = 2; // Settings Page
                option_num = 0;
                break;
            }
            break;
        case 2: // // Settings Page
            switch ((opt % number_picks_Settings))
            {
            case 0: // Option
                ConnectWifi(opt % number_picks_ConnectWifi, "");
                PageNumber = 3; // Settings Page
                option_num = 0;
                break;
            case 1: // Option
                break;
            case 2: // Option
                break;
            case 3:
                break;
            }
            break;
        case 3: // // Send Message Page
            SendMessage("");
            PageNumber = 4; // Send Message Pages
            option_num = 0;
            break;
        }
    }
    else
    {
        switch (PageNumber)
        {
        case 1: // Main Page
            MainMenu(opt, false);
            break;
        case 2: // Settings Page
            Settings(opt, false);
            break;
        case 3: // ConnectWifi Page
            ConnectWifi(opt, "");
            break;
        case 4: // ConnectWifi Page
            SendMessage("");
            break;
        }
    }
    UTILS.delay_ms(200);
}

