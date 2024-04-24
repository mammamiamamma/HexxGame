//
// Created by Ilya on 21.03.2024.
//
#include "SFML/Graphics.hpp"
#include "Button.h"
#ifndef HEXXGAME_BUTTONGENERATOR_H
#define HEXXGAME_BUTTONGENERATOR_H


class ButtonGenerator {
private:
    sf::Font font;
    sf::Color color;
    int fontSize;


public:
    ButtonGenerator() {
        this->fontSize = 50; //default value or smth
        this->color = sf::Color::White;
    }
    ButtonGenerator(sf::Font& font, int fontSize) : font(font), fontSize(fontSize){}
    ButtonGenerator(sf::Font& font, sf::Color& color, int fontSize) : font(font), color(color), fontSize(fontSize){}

    void setFont(const sf::Font& fontNew){
        this->font = fontNew;
    }

    void setColor(const sf::Color& colorNew){
        this->color = colorNew;
    }

    Button getButton(const string& text, sf::Vector2f size, sf::Vector2f coords);
    Button getButton(const string& text, int fontSizeNew, sf::Vector2f size);

    Button getButton(const string& text, int fontSizeNew, sf::Vector2f size,  sf::Vector2f coords);

//    Button getButton(sf::Vector2<float> size, sf::Vector2<float> coords);
    Button getLinelessButton(const string& text, int fontSizeNew, sf::Vector2f size);
    Button getButton(sf::Vector2<float> size, sf::Vector2<float> coords, const sf::Color& color);

    Button getButton(sf::Vector2<float> size);
};


#endif //HEXXGAME_BUTTONGENERATOR_H
