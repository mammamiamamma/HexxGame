#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
//#include "Player.h"
#include "StoneHelper.h"
#include "SFML/Graphics.hpp"

using namespace std;
namespace fs = std::filesystem;
class Player;

class HexBoard {
private:
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

public:
    static const int BOARD_SIZE = 9;
    static int freeSpaces;
    vector<vector<char>> board = {
            { ' ',' ',' ',' ','0',' ',' ',' ',' '},
            { ' ',' ','0','0','0','0','0',' ',' '},
            { '0','0','0','0','0','0','0','0','0'},
            { '0','0','0','0',' ','0','0','0','0'},
            { '0','0','0','0','0','0','0','0','0'},
            { '0','0','0',' ','0',' ','0','0','0'},
            { '0','0','0','0','0','0','0','0','0'},
            { ' ','0','0','0','0','0','0','0',' '},
            { ' ',' ',' ','0','0','0',' ',' ',' '}
    };
//    vector<vector<char>> board = {
//            { ' ',' ',' ',' ','0',' ',' ',' ',' '},
//            { ' ',' ','0','0','0','0','0',' ',' '},
//            { '0','0','0','0','0','0','0','0','0'},
//            { '0','0','0','0',' ','0','0','0','0'},
//            { '0','0','0','0','0','0','0','0','0'},
//            { '0','0','0',' ','0',' ','0','0','0'},
//            { '0','0','0','0','0','2','1','2','0'},
//            { ' ','0','0','0','0','1','X','0',' '},
//            { ' ',' ',' ','0','0','0',' ',' ',' '}
//    };
    HexBoard(const HexBoard& other)
            : board(other.board) {}
    HexBoard();

    void putStone(int y, int x, Player& p);
    /**
     * Puts a stone to the location (y,x) on the board
     * @param y the y coordinate of the board
     * @param x the x coordinate of the board
     * @param p the player who puts the stone
     */

    void putStone(int y, int x);
    /**
     * Puts a stone to the location (y,x) on the board
     * The difference between two putStone methods is that this one
     * is used to erase values from the board
     * @param y the y coordinate of the board
     * @param x the x coordinate of the board
     */

    bool validMove(int posy, int posx, int desty, int destx, Player& p);
    /**
     * checks whether the move (posy, posx) -> (desty, destx) is valid
     * @param posy starting y coordinate
     * @param posx starting x coordinate
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Player who makes the move
     * @return true if the move is valid, false otherwise
     */

    void closeMove(int desty, int destx, Player& p);
    /**
     * makes a close move (posy, posx) -> (desty, destx)
     * invoking only the first putStone method (tu duplicate a stone to the destination position)
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Player who makes the move
     */

    void jumpMove(int posy, int posx, int desty, int destx, Player& p);
    /**
     * makes a jump move (posy, posx) -> (desty, destx)
     * invokes both putStone methods to put a stone to the destination position
     * and to erase a stone from the starting position
     * @param posy origin y coordinate
     * @param posx origin x coordinate
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Player who makes the move
     */

    bool makeAMove(int posy, int posx, int desty, int destx, Player& p, Player& opponent);
    /**
     * "Qualifies" a move to define whether it a close move or a jump move
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Player who makes the move
     * @param opponent Opponent player, used to turn their pieces friendly
     */

    void turnPiecesFriendly(int desty, int destx, Player& p, Player& opponent);
    /**
     * Turns pieces around the destination position to friendly
     * @param desty destination y coordinate
     * @param destx destination x coordinate
     * @param p Player who makes the move
     * @param opponent Opponent player, used their turn pieces friendly
     */
    std::vector<std::vector<int>> createPossibleMoves(int posx, int posy);
};
#endif // HEXBOARD_H