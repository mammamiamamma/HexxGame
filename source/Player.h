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
    std::vector<std::vector<int>> neighborPosForOdd = {
            {0,-1},{0,1},{1,0},{-1,0},
            {1,-1},{-1,-1}
    };
    std::vector<std::vector<int>> neighborPosForEven = {
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
            for (auto &possibleMove: position.first % 2 == 0 ? possibleMovesForEven : possibleMovesForOdd) {
                int newx = position.first + possibleMove[0];
                int newy = position.second + possibleMove[1];
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
    std::vector<std::vector<int>> createPossibleMoves(HexBoard &hb, int posx, int posy) {
        std::vector<std::vector<int>> piecestolight;
        for (std::vector<int> vec : posx%2==1 ? possibleMovesForOdd : possibleMovesForEven) {
            int newx = posx + vec[0];
            int newy = posy + vec[1];
            if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
                continue;
            if (hb.board[newy][newx] == '0') {
                piecestolight.push_back({ newx, newy });
            }
        }
        return piecestolight;
    }
//    void findAllMoves(vector<int>& )
};

#endif  // PLAYER_H