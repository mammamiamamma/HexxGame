#ifndef BOT_H
#define BOT_H

#include "Player.h"
#include "HexBoard.h"
#include <vector>
#include <utility>

class Bot : public Player {
private:
public:
    Bot();
    explicit Bot(Stone stone);
    virtual std::vector<int> makeMove(HexBoard &hb);
    /**
     * initializes the bot's move utilizing information from other methods
     */
    /**
     * finds all positions of the bot's stones
     */
    /**
     * creates all possible moves of the bot's stones
     */
    virtual std::vector<int> evaluatemoves(HexBoard &hb);
    /**
     * evaluates the moves of the bot's stones
     */
    static bool isMoveCloser(int posx, int posy, int chosenx, int choseny, int origposx, int origposy);
    virtual int fakeTPF(int desty, int destx, Player& p, HexBoard &hb);
    /**
     * simulates the move to find the most profitable move
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Bot's opponent to turn their pieces to bot's pieces
     */
    [[nodiscard]] bool isBot() const override;
    /**
     * (overrides the method in Player) returns whether the player is a bot or not
     */
};

#endif // BOT_H