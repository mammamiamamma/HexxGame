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
#include "ui/ButtonGenerator.h"

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
    sf::ContextSettings& windowSettings;
public:
    bool isFullscreen;
    std::vector<std::vector<sf::CircleShape>> shapeBoard;
    sf::RenderWindow& window;
    sf::RectangleShape background;
    sf::Font& font;
    ButtonGenerator generator;
//    sf::WindowHandle handle(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Fullscreen)
    float WINDOW_SIZE_X; //made it float to silence the conversion warnings in Menu.cpp
    float WINDOW_SIZE_Y;

    Application(sf::RenderWindow &windowRef, sf::RectangleShape &bg, sf::Font &font, sf::ContextSettings &windowSettings1) :
    window(windowRef), shapeBoard(9, std::vector<sf::CircleShape>(9)), background(std::move(bg)), font(font),
    windowSettings(windowSettings1) {
        WINDOW_SIZE_X = (float) window.getSize().x;
        WINDOW_SIZE_Y = (float) window.getSize().y;
        isFullscreen = false;
        generator = ButtonGenerator(font, 50);
        generator.setColor(sf::Color::White);
    }

    void changeState();
    void initiateShapeBoard(HexBoard& hb);
    /**
     * initates and displays the shape board in the window
     * @param hb the board to display
     */
    void updateShapeBoard(HexBoard &hb);
    /**
     * updates and displays the shape board in the window
     * @param hb the board to display
     */
    void displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb, int posx, int posy);
    void displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb);
//    static sf::RectangleShape createButton(const sf::Vector2f& size);
//    static sf::RectangleShape createButton(const sf::Vector2f& size, const sf::Color& color);
//    static sf::RectangleShape createButton(const sf::Vector2f& size, bool drawOutline); //obviously change later
//    static sf::Vector2f getTextPosition(sf::Text &text, const sf::RectangleShape &rectangle);
//    sf::Text createText(const std::string& textContent, unsigned int characterSize, sf::RectangleShape& targetButton) const;
    sf::Text createText(const string &textContent, unsigned int characterSize) const;
    /**
     * displays the possible moves of a piece
     * by highlighting them
     * @param posx the x position of the piece
     * @param posy the y position of the piece
     * @param indicator whether to highlight the possible moves or not
     */

//    void drawButton(const sf::RectangleShape& b, const sf::Text& text);
    void drawButton(Button& button);
    void drawButton(vector<Button>& button);
//    void drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr);
    void drawMenu(std::vector<Button>& buttArr);
    void drawMenu(std::vector<Button> &buttArr,
                  std::vector<Button>& controlButtons,
                  int& pageNum,
                  string& totalPagesStr,
                  bool isFirstPage,
                  bool isLastPage,
                  bool isButtonNeeded);
    void displayShapes(vector<Button>& buttArr);
    sf::Color getTileColor(const char& tileContent);
    sf::Color getBorderColor(const char& tileContent);
    static bool isClickedInsideTile(sf::Vector2f point, const sf::CircleShape& tile);
    /**
     * moved the methods above to this class because i wanted to refactor it to be sort of a rendering class, so there's that
     */
};

#endif // APPLICATION_H