#include "Player.h"
#include "HexBoard.h"

Player::Player() {
    score = 3;
}

Player::Player(Stone stone1) : stone(stone1) {}

Stone Player::getStone() const {
    return stone;
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int num) {
    this->score = num;
}

bool Player::isBot() const {
    return false;
}

void Player::findAllPositions(const HexBoard &hb) {
    for (int i = 0; i < HexBoard::BOARD_SIZE; i++) {
        for (int j = 0; j < HexBoard::BOARD_SIZE; j++) {
            if (hb.board[i][j] == StoneHelper::getName(stone)) {
                positions.emplace_back(j, i); // j - x, i - y
            }
        }
    }
}

void Player::createPossibleMoves(const HexBoard &hb) {
    findAllPositions(hb);
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

std::vector<std::vector<int>> Player::createPossibleMoves(const HexBoard &hb, int posx, int posy) {
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

