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

