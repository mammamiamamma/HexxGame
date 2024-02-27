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
    std::vector<std::vector<int>> possibleMovesForEven = {
            {0,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0},
            {0,-2}, {-2,-1}, {-2, 0}, {-2, 1}, {-1, 2}, {0, 2}, {1,2}, {2, 1}, {2, 0}, {2,-1}, {1,-1},
            {-1,-1},
    };
    std::vector<std::vector<int>> possibleMovesForOdd = {
            {0,-1}, {-1,-1}, {-1,0}, {0,1}, {1,0}, {1,-1},
            {0,-2}, {-1,-2}, {-2,-1}, {-2,0}, {-2,1}, {0,2}, {2,1}, {2,0}, {2,-1}, {1,-2},
            {-1,1}, {1,1}
    };
    std::vector<std::pair<int, int>> positions;
    std::vector<std::vector<std::pair<int, int>>> positionsandmoves;

    Player();
    explicit Player(Stone stone1);
    virtual ~Player() = default;

    Stone getStone() const;
    /**
     * returns the Stone of the player
     */
    int getScore() const;
    /**
     * returns the score of the player
     */
    void setScore(int score);
    /**
     * sets/changes the score of the player
     */
    virtual bool isBot() const;
    /**
     * returns whether the player is a bot or not
     */
    void findallpositions(HexBoard &hb) {
        for (int i = 0; i < HexBoard::BOARD_SIZE; i++) {
            for (int j = 0; j < HexBoard::BOARD_SIZE; j++) {
                if (hb.board[i][j] == StoneHelper::getName(stone)) {
                    positions.emplace_back(j, i); // j - x, i - y
                }
            }
        }
    }

    void createPossibleMoves(HexBoard &hb) {
        findallpositions(hb);
        for (auto& position : positions) {
            if (position.first%2==0){
                for (auto& possiblemove : possibleMovesForEven) {
                    int newx = position.first + possiblemove[0];
                    int newy = position.second + possiblemove[1];
                    if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
                        continue;
                    if (hb.board[newy][newx] == ' ')
                        continue;
                    if (hb.board[newy][newx] == '0') {
                        std::vector<std::pair<int, int>> temp = {position, {newx, newy}};
                        positionsandmoves.emplace_back(temp);
                    }
                }
            } else {
                for (auto &possiblemove: possibleMovesForOdd) {
                    int newx = position.first + possiblemove[0];
                    int newy = position.second + possiblemove[1];
                    if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
                        continue;
                    if (hb.board[newy][newx] == ' ')
                        continue;
                    if (hb.board[newy][newx] == '0') {
                        std::vector<std::pair<int, int>> temp = {position, {newx, newy}};
                        positionsandmoves.emplace_back(temp);
                    }
                }
            }
        }
    }
};

#endif  // PLAYER_H