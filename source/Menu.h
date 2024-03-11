//
// Created by Ilya on 20.02.2024.
//

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "Application.h"

// note for me: return -1 for "go back", return 0 for i dunno exit i guess (yeah its returned when default and what not), return 1 (in case of loadgame its a successful loading game)
class Menu {
public:
    explicit Menu(Application& app);
    int launchMenu();

private:
    Application& app;
    std::vector<sf::RectangleShape> controlButtons;
    std::vector<sf::Text> controlButtonsText;

    bool isFirstPage = true;
    bool isLastPage = false;
    bool isButtonNeeded = false;

    /**
    * launches the new game menu
    * @return 0 if the game is over
    */
    int launchNewGameMenu();
    int launchLoadGameMenu();
    int launchSettingsMenu();
    static std::vector<std::string> getFileNames();
    static std::vector<std::string> split(const std::string& s);
    /**
    * the function below initializes the game with a bot
    * @return 0 if the game is over
    */
    int playWithBot();
    /**
    * the function below initializes the game with another player
    * @return 0 if the game is over
    */
    int playWithHuman();

    std::vector<sf::Text> getTextForButton(const std::string& filename);

    static int loadGame();

    void initializeMainMenuButtons(vector<sf::RectangleShape>& buttArr, vector<sf::Text>& textArr);
    void initializeNewGameButtons(vector<sf::RectangleShape> &buttArr, vector<sf::Text> &textArr);
    void initializeSettingsButtons(vector<sf::RectangleShape> &buttArr, vector<sf::Text> &textArr, bool isFullscreen);

    static int registerMouseMove(vector<sf::RectangleShape> &buttArr, sf::Vector2f mouse_pos, int ind);

    static int registerMouseClick(vector<sf::RectangleShape> &buttArr, sf::Vector2f mouse_pos);

    void updatePageButtons(int pageNum, int totalPages);
    void ClearControlButtons();
    void ResetMenuState();

};

#endif // MENU_H
