/*
 * Pages.h
 *
 *  Created on: Jan 3, 2021
 *      Author: bakialmaci
 */

#ifndef PAGES_H_
#define PAGES_H_

class Pages
{
public:
    Pages();
    virtual ~Pages();

    // Page utils
    void Initialize();
    void Picker(int opt, bool btn_state);
    void Typing(int opt, const char *string);

    // Pages
    void Intro();
    void MainMenu(int opt, bool init_state);
    void Settings(int opt, bool init_state);
    void ConnectWifi(int opt, const char *string);

};

#endif /* PAGES_H_ */
