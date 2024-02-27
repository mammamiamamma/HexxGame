//
// Created by Ilya on 20.02.2024.
//

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow &window, sf::Font &font, sf::RectangleShape &bg);

    void runMenu();

private:
    sf::RenderWindow& window;
    sf::Font& font;
    sf::RectangleShape background;

    void drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr);
    int launchNewGameMenu();
    int launchLoadGameMenu();
    static std::vector<std::string> getFileNames();
    std::vector<std::string> split(const std::string& s, char delimiter);
    /**
    * the function below initializes the game with a bot
    * @return 0 if the game is over
    */
    void playwithbot();
    /**
    * the function below initializes the game with another player
    * @return 0 if the game is over
    */
    void playwithhuman();
    int launchMenu();

    std::vector<sf::Text> getTextForButton(const std::string& filename);

    void drawButton(const sf::RectangleShape& b, const sf::Text& text);

    void drawMenu(const std::vector<sf::RectangleShape> &buttArr, const std::vector<sf::Text> &textArr, int pageNum);

    int loadGame();
};

#endif // MENU_H
