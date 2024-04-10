#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "StoneHelper.h"
#include "HexBoard.h"
//class HexBoard;
class Player {
private:
    int score = 3;
protected:
    Stone stone;
public:
    const std::vector<std::vector<int>> possibleMovesForEven = {
            {0,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0},
            {0,-2}, {-2,-1}, {-2, 0}, {-2, 1}, {-1, 2}, {0, 2}, {1,2}, {2, 1}, {2, 0}, {2,-1}, {1,-1},
            {-1,-1},
    };
    const std::vector<std::vector<int>> possibleMovesForOdd = {
            {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,0}, {1,-1},
            {0,-2}, {-1,-2}, {-2,-1}, {-2,0}, {-2,1}, {0,2}, {2,1}, {2,0}, {2,-1}, {1,-2},
            {-1,1}, {1,1}
    };
    const std::vector<std::vector<int>> neighborPosForOdd = {
            {0,-1},{0,1},{1,0},{-1,0},
            {1,-1},{-1,-1}
    };
    const std::vector<std::vector<int>> neighborPosForEven = {
            {0,-1}, {-1,0}, {-1,1},
            {0,1}, {1,1}, {1,0}
    };
    std::vector<std::pair<int, int>> positions;
    std::vector<std::vector<std::pair<int, int>>> positionsandmoves;

    Player();
    explicit Player(Stone stone1);
    virtual ~Player() = default;

    [[nodiscard]] Stone getStone() const;
    /**
     * returns the Stone of the player
     */
    [[nodiscard]] int getScore() const;
    /**
     * returns the num of the player
     */
    void setScore(int num);
    /**
     * sets/changes the score of the player
     */
    [[nodiscard]] virtual bool isBot() const;
    /**
     * returns whether the player is a bot or not
     */
    void findAllPositions(const HexBoard &hb);
    void createPossibleMoves(const HexBoard &hb);
    std::vector<std::vector<int>> createPossibleMoves(const HexBoard &hb, int posx, int posy);
};

#endif  // PLAYER_H