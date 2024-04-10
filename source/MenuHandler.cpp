//
// Created by Ilya on 23.03.2024.
//

#include "MenuHandler.h"

MenuHandler::MenuHandler(Menu &menu) : menu(menu){}
void MenuHandler::handleMainMenu() {
    int choice;
    bool shouldExit = false;
    while (!shouldExit) {
//        menu.drawMainMenu(); // Maybe I should separate the methods to be smth like this?
        choice = menu.launchMenu();
        switch (choice) {
            case 0: { //new game button
                menu.app.window.clear();
                handleNewGameMenu();
                break;
            }
            case 1: { //load game button
                handleLoadGameMenu();
                break;
            }
            case 2: { //settings button
                handleSettingsMenu();
                break;
            }
            case 3: //exit button
            case -1: { //window closed
                shouldExit = true;
                break;
            }
            default: {
                break;
            }
        }
    }
}

void MenuHandler::handleNewGameMenu() {
    int choice = menu.launchNewGameMenu();
    switch (choice){
        case 0: {
            menu.initializeGame(false, true); //introduce enum GAME_MODE?
            break;
        }
        case 1: {
            menu.initializeGame(false, false);
            break;
        }
        default:
            break;
    }
}

void MenuHandler::handleLoadGameMenu() {
    int choice = menu.launchLoadGameMenu();
    menu.ClearControlButtons();
    menu.ResetMenuState();
    switch (choice) {
        case 1: {
            menu.initializeGame(true, false); // isPVP doesnt matter in this case
            break;
        }
        default: return;
//        case -1: {
//            break;
//        }
    }
}

void MenuHandler::handleSettingsMenu() {
    int choice = menu.launchSettingsMenu();
    switch (choice){
        case -1: {
            menu.ClearControlButtons();
            menu.ResetMenuState();
            break;
        }
        //to be expanded
    }
}

