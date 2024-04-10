//
// Created by Ilya on 23.03.2024.
//

#ifndef HEXXGAME_MENUHANDLER_H
#define HEXXGAME_MENUHANDLER_H

#include "Menu.h"

class MenuHandler {
private:
    Menu& menu;
public:
    explicit MenuHandler(Menu& menu);
    void handleMainMenu();
    void handleNewGameMenu();
    void handleLoadGameMenu();
    void handleSettingsMenu();
};

#endif //HEXXGAME_MENUHANDLER_H
