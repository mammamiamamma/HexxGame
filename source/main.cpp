#include <iostream>
#include <fstream>
#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "Application.h"

using namespace std;

int main() {
    sf::Font font;
    sf::Texture texture;
    if (!texture.loadFromFile("rsc/pics/spacebackground.jpg")) {
        cout << "oops no background" << endl;
        return 1; // Exit if the background cannot be loaded
    }
    if (!font.loadFromFile("rsc/fonts/venus_font.otf")) {
        cout << "oops no font" << endl;
        return 1; // Exit if the font cannot be loaded
    }
//    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Fullscreen); //need to try and make the game fullscreen while we're at it
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
//    window.create(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Fullscreen);

    sf::ContextSettings conSettings;
    conSettings.antialiasingLevel = 16;

    window.create(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, sf::State::Windowed, conSettings);
    sf::RectangleShape background({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    background.setTexture(&texture);
    Application app(window, background, font, conSettings);
    Menu menu(app);
    menu.launchMenu();

    return 0;
}

