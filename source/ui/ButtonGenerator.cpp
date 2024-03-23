//
// Created by Ilya on 21.03.2024.
//

#include "ButtonGenerator.h"



Button ButtonGenerator::getButton(const string& text, const sf::Vector2f size, const sf::Vector2f coords){
    Button button(size, color, coords);
    sf::Text buttonText(font, text, fontSize);
    buttonText.setPosition(button.getTextPosition(buttonText));
    button.addText(buttonText);
    return button;
}

Button ButtonGenerator::getButton(const string& text, int fontSizeNew, const sf::Vector2f size){
    Button button(size, color);
    sf::Text buttonText(font, text, fontSizeNew);
    buttonText.setPosition(button.getTextPosition(buttonText));
    button.addText(buttonText);
    return button;
}

Button ButtonGenerator::getButton(const string& text, int fontSizeNew, const sf::Vector2f size, const sf::Vector2f coords){
    Button button(size, color, coords);
    sf::Text buttonText(font, text, fontSizeNew);
    buttonText.setPosition(button.getTextPosition(buttonText));
    button.addText(buttonText);
    return button;
}

//Button ButtonGenerator::getButton(const sf::Vector2f size, const sf::Vector2f coords){
//    Button button(size, color, coords);
//    return button;
//}

Button ButtonGenerator::getButton(sf::Vector2<float> size, sf::Vector2<float> coords){
    Button button(size, color, coords);
    return button;
}

Button ButtonGenerator::getButton(sf::Vector2<float> size){
    Button button(size, color);
    return button;
}

Button ButtonGenerator::getButton(sf::Vector2<float> size, sf::Vector2<float> coords, const sf::Color& colorNew){
    Button button(size, colorNew, coords);
    return button;
}