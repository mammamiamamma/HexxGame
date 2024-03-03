//
// Created by Ilya on 20.02.2024.
//

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Application.h"

class Menu {
public:
    Menu(sf::Font &font, Application& app);
    void runMenu();
    bool isFirstPage = true;
    bool isLastPage = false;
    bool isButtonNeeded = false;

private:
    Application& app;
    sf::Font& font;
    std::vector<sf::RectangleShape> controlButtons;
    std::vector<sf::Text> controlButtonsText;

    int launchNewGameMenu();
    int launchLoadGameMenu();
    static std::vector<std::string> getFileNames();
    std::vector<std::string> split(const std::string& s, char delimiter);
    /**
    * the function below initializes the game with a bot
    * @return 0 if the game is over
    */
    int playwithbot();
    /**
    * the function below initializes the game with another player
    * @return 0 if the game is over
    */
    int playwithhuman();
    int launchMenu();

    std::vector<sf::Text> getTextForButton(const std::string& filename);

    static int loadGame();
};

#endif // MENU_H
