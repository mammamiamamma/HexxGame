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
    std::vector<std::vector<int>> possibleMovesForEven = {
            {0,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0},
            {0,-2}, {-2,-1}, {-2, 0}, {-2, 1}, {-1, 2}, {0, 2}, {1,2}, {2, 1}, {2, 0}, {2,-1}, {1,-1},
            {-1,-1},
    };
    std::vector<std::vector<int>> possibleMovesForOdd = {
            {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,0}, {1,-1},
            {0,-2}, {-1,-2}, {-2,-1}, {-2,0}, {-2,1}, {0,2}, {2,1}, {2,0}, {2,-1}, {1,-2},
            {-1,1}, {1,1}
    };

    std::vector<std::vector<int>> piecestolight;

public:
    sf::RenderWindow& window;
    std::vector<std::vector<sf::CircleShape>> shapeBoard;
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
    HexBoard& b;
    sf::RectangleShape background;

    Application(sf::RenderWindow& windowRef, HexBoard& boardRef, sf::RectangleShape bg)
        : window(windowRef), b(boardRef), shapeBoard(9, std::vector<sf::CircleShape>(9)), background(std::move(bg)) {}

    void initiateShapeBoard(HexBoard& hb);
    /**
     * displays the board in the window
     * @param hb the board to display
     */
    bool displayPossibleMoves(int posx, int posy, bool indicator);
    /**
     * displays the possible moves of a piece
     * by highlighting them
     * @param posx the x position of the piece
     * @param posy the y position of the piece
     * @param indicator whether to highlight the possible moves or not
     */
    void displayShapes();
    sf::Color getTileColor(const char& tileContent);
    sf::Color getBorderColor(const char& tileContent);
    static bool isClickedInsideTile(sf::Vector2f point, const sf::CircleShape& tile);

};

#endif // APPLICATION_H