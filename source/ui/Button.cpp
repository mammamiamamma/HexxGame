//
// Created by Ilya on 21.03.2024.
//

#include "Button.h"

sf::Vector2f Button::getTextPosition(const sf::Text &text) {
    sf::FloatRect textBounds = text.getLocalBounds();
    float xPosition = button.getPosition().x + (button.getSize().x - textBounds.width) / 2 - textBounds.left;
    float yPosition = button.getPosition().y + (button.getSize().y - textBounds.height) / 2 - textBounds.top;
    return {xPosition, yPosition};
}

Button::Button(sf::Vector2f size, const sf::Color& color){
    button.setSize(size);
    button.setFillColor(sf::Color(0,0,0,192));
    button.setOutlineThickness(5.f);
    button.setOutlineColor(color);
}

Button::Button(sf::Vector2f size, const sf::Color& color, const sf::Vector2f coords){
    button.setSize(size);
    button.setPosition(coords);
    button.setFillColor(sf::Color(0,0,0,192));
    button.setOutlineThickness(5.f);
    button.setOutlineColor(color);
}

void Button::setPosition(sf::Vector2f coords){
    button.setPosition(coords);
    for (auto& text : buttonText){
        text.setPosition(getTextPosition(text));
    }
}

void Button::setOutlineColor(sf::Color color){
    button.setOutlineColor(color);
}

sf::RectangleShape& Button::getShape(){
    return button;
}

vector<sf::Text>& Button::getText(){
    return buttonText;
}

void Button::addText(sf::Text& text){
    buttonText.emplace_back(text);
}

void Button::addText(vector<sf::Text>& textArr){
    for (const auto& text : textArr){
        buttonText.emplace_back(text);
    }
}