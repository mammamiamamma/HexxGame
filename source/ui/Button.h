//
// Created by Ilya on 21.03.2024.
//

#include "SFML/Graphics.hpp"

#ifndef HEXXGAME_BUTTON_H
#define HEXXGAME_BUTTON_H

using namespace std;
class Button {
    private:
    sf::RectangleShape button;
    sf::Color buttonColor;
    vector<sf::Text> buttonText;

    public:
    bool isActiveButton = true;
    Button() = default;
    sf::Vector2f getTextPosition(const sf::Text& text);
    Button(sf::Vector2f size, const sf::Color& color);
    Button(sf::Vector2f size, const sf::Color& color, sf::Vector2f coords);
    void setPosition(sf::Vector2f coords);
    void setOutlineColor(sf::Color color);
    void addText(sf::Text& text);
    void addText(vector<sf::Text>& textArr);
    sf::RectangleShape& getShape();
    vector<sf::Text>& getText();
};


#endif //HEXXGAME_BUTTON_H
