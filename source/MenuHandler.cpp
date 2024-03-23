//
// Created by Ilya on 23.03.2024.
//

#include "MenuHandler.h"

MenuHandler::MenuHandler(Menu &menu) : menu(menu){}

void MenuHandler::handleMainMenu() {
    int choice = -1;
    bool shouldExit = false;
    while (choice != 3 && !shouldExit) {
//        menu.drawMainMenu(); // Assuming you have a method like this
        choice = menu.launchMenu();
        switch (choice) {
            case 0: {
                menu.app.window.clear();
                handleNewGameMenu();
                break;
            }
            case 1: {
                handleLoadGameMenu();
                break;
            }
            case 2: {
                handleSettingsMenu();
                break;
            }
            case 3: {
                shouldExit = true;
                break;
            }
            default: {
                // Handle invalid choice or refresh the menu
                break;
            }
        }
    }
}

void MenuHandler::handleNewGameMenu() {
    bool shouldExit = false;
    while (!shouldExit){
        int choice = menu.launchNewGameMenu();
        switch (choice){
            case 0:{
                menu.playWithHuman();
                break;
            }
            case 1:{
                menu.playWithBot();
                break;
            }
            default:
                shouldExit = true;
                break;
        }
    }
}

void MenuHandler::handleLoadGameMenu() {
    int choice = menu.launchLoadGameMenu();
    menu.ClearControlButtons();
    menu.ResetMenuState();
    switch (choice){
        case 1: {
            handleLoadGame();
            break;
        }
        default: return;
//        case -1: {
//            break;
//        }
    }
}

void MenuHandler::handleLoadGame() {
    int choice = Menu::loadGame();
    switch (choice){
        case 1: {
            menu.playWithHuman();
            break;
        }
        case 2: {
            menu.playWithBot();
            break;
        }
        default: return;
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
    }
}

