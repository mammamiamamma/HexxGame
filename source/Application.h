#ifndef APPLICATION_H
#define APPLICATION_H

#include "SFML/Graphics.hpp"
#include <map>
#include <iostream>
#include <array>
#include <utility>
#include <vector>
#include <sstream>
#include <cmath>
#include "HexBoard.h"

class Application {
private:
    const std::map<char, sf::Color> colorMap = {
            {'R', sf::Color(255, 0, 0)},
            {'B', sf::Color(0, 0, 255)},
            {'0', sf::Color(170, 170, 170)},
            {' ', sf::Color(0,0,0,0)},
    };
    const std::map<char, sf::Color> borderMap = {
            {'R', sf::Color(120, 0, 0)},
            {'B', sf::Color(0, 0, 120)},
            {'0', sf::Color(70, 70, 70)},
            {' ', sf::Color(0,0,0,0)},
    };
public:
    std::vector<std::vector<sf::CircleShape>> shapeBoard;
    sf::RenderWindow& window;
    sf::RectangleShape background;

    Application(sf::RenderWindow& windowRef, sf::RectangleShape bg)
        : window(windowRef), shapeBoard(9, std::vector<sf::CircleShape>(9)), background(std::move(bg)) {
    }

    void initiateShapeBoard(HexBoard& hb);
    /**
     * displays the board in the window
     * @param hb the board to display
     */
    void updateShapeBoard(HexBoard &hb);

//    bool displayPossibleMoves(int posx, int posy, bool indicator);
    void displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb, int posx, int posy);
    void displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb);
    /**
     * displays the possible moves of a piece
     * by highlighting them
     * @param posx the x position of the piece
     * @param posy the y position of the piece
     * @param indicator whether to highlight the possible moves or not
     */

    void drawButton(const sf::RectangleShape& b, const sf::Text& text);
    void drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr);
    void drawMenu(const std::vector<sf::RectangleShape> &buttArr, const std::vector<sf::Text> &textArr, const std::vector<sf::RectangleShape>& controlButtons, const std::vector<sf::Text>& controlButtonsText, sf::Font& font, int& pageNum, string& totalPagesStr, bool isFirstPage,
                  bool isLastPage,
                  bool isButtonNeeded);
    void displayShapes();
    sf::Color getTileColor(const char& tileContent);
    sf::Color getBorderColor(const char& tileContent);
    static bool isClickedInsideTile(sf::Vector2f point, const sf::CircleShape& tile);

};

#endif // APPLICATION_H