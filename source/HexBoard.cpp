#include "HexBoard.h"
#include "Player.h"
#include <fstream>
#include <filesystem>


void HexBoard::putStone(int y, int x, Player& p) {
    char stone;
    if (p.getStone() == Stone::BLUE) stone = 'B';
    else stone = 'R';
    board[y][x] = stone;
}

void HexBoard::putStone(int y, int x) {
    board[y][x] = '0';
}

bool HexBoard::validMove(int posy, int posx, int desty, int destx, Player& p) {
    if (board[posy][posx]!= StoneHelper::getName(p.getStone())) return false;
    if (desty < 0 || desty >= BOARD_SIZE || destx < 0 || destx >= BOARD_SIZE) return false;
    if (posy < 0 || posy >= BOARD_SIZE || posx < 0 || posx >= BOARD_SIZE) return false;
    if (board[desty][destx] == ' ' || board[posy][posx] == ' ') return false;
    if (board[posy][posx] == '0') return false;
    else return true;
}

void HexBoard::closeMove(int desty, int destx, Player& p) {
    putStone(desty, destx, p);
    p.setScore(p.getScore()+1);
    freeSpaces--;
}

void HexBoard::jumpMove(int posy, int posx, int desty, int destx, Player& p) {
    putStone(desty, destx, p);
    putStone(posy, posx);
}

bool HexBoard::makeAMove(int posy, int posx, int desty, int destx, Player& p, Player& opponent) {
    if (validMove(posy, posx, desty, destx, p)) {
        if (abs(posy - desty) == 1 && abs(posx - destx) == 0) {
            closeMove(desty, destx, p);
            turnPiecesFriendly(desty, destx, p, opponent);
            return true; //both
        } else if (abs(posy - desty) == 0 && abs(posx - destx) == 1) {
            closeMove(desty, destx, p);
            turnPiecesFriendly(desty, destx, p, opponent);
            return true; //both
        } else if (abs(posy - desty) == 2 && abs(posx - destx) == 0) {
            jumpMove(posy, posx, desty, destx, p);
            turnPiecesFriendly(desty, destx, p, opponent);
            return true; //both
        } else if (abs(posy - desty) == 0 && abs(posx - destx) == 2) {
            jumpMove(posy, posx, desty, destx, p);
            turnPiecesFriendly(desty, destx, p, opponent);
            return true; //both
        } else if (abs(posy - desty) == 1 && abs(posx - destx) == 2) {
            jumpMove(posy, posx, desty, destx, p);
            turnPiecesFriendly(desty, destx, p, opponent);
            return true; //both
        }
        if (posx%2==1) {
            if (posy - desty == -1 && abs(posx - destx) == 1) {
                jumpMove(posy, posx, desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            }
            else if (posy - desty == 1 && abs(posx - destx) == 1) {
                closeMove(desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            }
            else if (abs(posx - destx) == 1 && posy - desty == 2){
                jumpMove(posy, posx, desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            }
        } else {
            if (posy - desty == -1 && abs(posx - destx) == 1){
                closeMove(desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            } else if (posy - desty == 1 && abs(posx - destx) == 1) {
                jumpMove(posy, posx, desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            } else if (abs(posx - destx) == 1 && posy - desty == -2){
                jumpMove(posy, posx, desty, destx, p);
                turnPiecesFriendly(desty, destx, p, opponent);
                return true;
            }
        }
    }
    return false;
}

void HexBoard::turnPiecesFriendly(int desty, int destx, Player& p, Player& opponent) {
    std::vector<std::vector<int>> neighborPos;
    if (destx % 2 == 0) {
        neighborPos = {
                {0,-1}, {-1,0}, {-1,1},
                {0,1}, {1,1}, {1,0}
        };
    } else {
        neighborPos = {
                {0,-1},{0,1},{1,0},{-1,0},
                {1,-1},{-1,-1}
        };
    }
    for (auto & neighborPo : neighborPos) {
        int newy = desty + neighborPo[1];
        int newx = destx + neighborPo[0];
        if (newy < 0 || newy >= BOARD_SIZE || newx < 0 || newx >= BOARD_SIZE) continue;
        if (board[newy][newx] == ' ') continue;
        if (board[newy][newx] == '0') continue;
        if (board[newy][newx] != StoneHelper::getName(p.getStone())) {
            board[newy][newx] = StoneHelper::getName(p.getStone());
            p.setScore(p.getScore() + 1);
            opponent.setScore(opponent.getScore() - 1);
        }
    }
}

int HexBoard::freeSpaces = 52;

std::vector<std::vector<int>> HexBoard::createPossibleMoves(int posx, int posy) {
    std::vector<std::vector<int>> piecestolight;
    if (posx%2==1){
        for (std::vector<int> vec : possibleMovesForOdd) {
            int newx = posx + vec[0];
            int newy = posy + vec[1];
            if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
                continue;
            if (board[newy][newx] == '0') {
                piecestolight.push_back({ newx, newy });
            }
        }
    } else {
        for (std::vector<int> vec : possibleMovesForEven) {
            int newx = posx + vec[0];
            int newy = posy + vec[1];
            if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
                    continue;
            if (board[newy][newx] == '0') {
                piecestolight.push_back({ newx, newy });
            }
        }
    }
    return piecestolight;
}

HexBoard::HexBoard() = default;