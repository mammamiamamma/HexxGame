#include <iostream>
#include <fstream>
#include "SFML/Graphics.hpp"
#include "Menu.h"

using namespace std;
//void loadgame(string &filename, HexBoard &hb, Player &p1, Player &p2, Stone &currstone) {
//    std::ifstream inputFile(filename);
//    if (inputFile.is_open()) {
//        for (int row = 0; row < HexBoard::BOARD_SIZE; ++row) {
//            std::string line;
//            if (!std::getline(inputFile, line)) {
//                break;  // Reached the end of the file
//            }
//            for (size_t col = 0; col < HexBoard::BOARD_SIZE; ++col) {
//                if (col >= line.length()) {
//                    break;  // Reached the end of the line
//                }
//                switch (line[col]) {
//                    case '0':
//                        hb.board[row][col] = ' ';
//                        break;
//                    case '1':
//                        hb.board[row][col] = '0';
//                        break;
//                    case 'R':
//                        hb.board[row][col] = 'R';
//                        break;
//                    case 'B':
//                        hb.board[row][col] = 'B';
//                        break;
//                    default:
//                        break;
//                }
//            }
//        }
//        string stone;
//        inputFile >> stone;
//        if (stone == "R") {
//            currstone = Stone::RED;
//        } else if (stone == "B") {
//            currstone = Stone::BLUE;
//        }
//        int score;
//        inputFile >> score;
//        HexBoard::freeSpaces = score;
//        inputFile >> score;
//        p1.setScore(score);
//        inputFile >> score;
//        p2.setScore(score);
//        inputFile >> stone;
//        if (stone == "Player") {
//            pvp = true;
//        } else {
//            pvp = false;
//        }
//        inputFile.close();
//        std::cout << "Board loaded successfully from file: " << filename << std::endl;
//    } else {
//        std::cerr << "Error: Cannot open the file" << std::endl;
//    }
//}

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

    Menu menu(window, font, background);
    menu.runMenu();

    return 0;
}

