#ifndef GAME_H
#define GAME_H

#include "HexBoard.h"
#include "Player.h"
#include "Application.h"
#include "Bot.h"
#include <filesystem>

namespace fs = std::filesystem;
class Game {
private:
    HexBoard& b;
    Player& p1;
    Player& p2;
    Bot* bot;
    Application& app;
    string filename;
    string filePath = "saves/";
    Stone currentStone;
    Stone otherStone;
public:
    explicit Game(HexBoard& b1, Player& p1, Player& p2, Application& app);

    void putDefaultStones();
    /**
     * initializes the start of the game
     */
    int startGame();
    int startLoadedGame(Stone currStone);
    void saveGame();

    int registerClickOnTile(sf::Vector2f mouse_pos);
    int registerClickOnButtons(sf::Vector2f mouse_pos);
    void registerKeyPressed(sf::Event::KeyEvent keyEvent);

    Player &getPlayerFromStone(Stone stone);

    void declareWinner();


};
#endif  // GAME_H
