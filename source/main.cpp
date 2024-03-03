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

    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::RectangleShape background({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
    background.setTexture(&texture);
    Application app(window, background);
    Menu menu(font, app);
    menu.runMenu();

    return 0;
}

