#include "Bot.h"
#include "HexBoard.h"
#include <random>

Bot::Bot() : Player() {}

Bot::Bot(Stone stone) : Player(stone) {}

std::vector<int> Bot::makeMove(const HexBoard &hb) {
    std::vector<int> move = {0, 0, 0, 0};
    if (positionsandmoves.empty()) createPossibleMoves(hb);
    move = evaluatemoves(hb);
    positions.clear();
    positionsandmoves.clear();
    return move;
}

std::vector<int> Bot::evaluatemoves(const HexBoard &hb) {
    std::vector<int> chosenmove;
    int maxprof = 0;
    int origposx = 0;
    int origposy = 0;
    int chosenx = 0;
    int choseny = 0;
    for (auto& pos : positionsandmoves) {
        int posx = pos[0].first;
        int posy = pos[0].second;
        int count = fakeTPF(pos[1].second, pos[1].first, hb);
        bool isBetterProfit = count > maxprof;
        bool isSameProfitButCloser = count == maxprof && isMoveCloser(posx, posy, chosenx, choseny, origposx, origposy);
        if (isBetterProfit || isSameProfitButCloser)
        {
            origposx = posx;
            origposy = posy;
            chosenx = pos[1].first;
            choseny = pos[1].second;
            maxprof = count;
        }
    }
    if (maxprof == 0) {
        if (!positionsandmoves.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, positionsandmoves.size() - 1);
            int randomIndex = dis(gen);
            origposx = positionsandmoves[randomIndex][0].first;
            origposy = positionsandmoves[randomIndex][0].second;
            chosenx = positionsandmoves[randomIndex][1].first;
            choseny = positionsandmoves[randomIndex][1].second;
            // Generate a random move until the conditions are satisfied
            bool decided = false;
            while (!decided){
                if (origposx%2==0){
                    if (
                            (abs(origposx-chosenx)==1 && origposy-choseny==1) ||
                            std::abs(origposx - chosenx) >= 2 ||
                            std::abs(origposy - choseny) >= 2
                            )
                    {
                        randomIndex = dis(gen);
                        origposx = positionsandmoves[randomIndex][0].first;
                        origposy = positionsandmoves[randomIndex][0].second;
                        chosenx = positionsandmoves[randomIndex][1].first;
                        choseny = positionsandmoves[randomIndex][1].second;
                    } else decided = true;
                } else {
                    if (
                            (abs(origposx-chosenx)==1 && origposy-choseny==-1) ||
                            std::abs(origposx - chosenx) >= 2 ||
                            std::abs(origposy - choseny) >= 2
                            )
                    {
                        randomIndex = dis(gen);
                        origposx = positionsandmoves[randomIndex][0].first;
                        origposy = positionsandmoves[randomIndex][0].second;
                        chosenx = positionsandmoves[randomIndex][1].first;
                        choseny = positionsandmoves[randomIndex][1].second;
                    } else decided = true;
                }
            }
        }
    }
    chosenmove.emplace_back(origposx);
    chosenmove.emplace_back(origposy);
    chosenmove.emplace_back(chosenx);
    chosenmove.emplace_back(choseny); //implement structs and whatnot
    return chosenmove;
}
bool Bot::isMoveCloser(int posx, int posy, int chosenx, int choseny, int origposx, int origposy) {
    int distanceCurrent = std::abs(origposx - chosenx) + std::abs(origposy - choseny);
    int distanceNew = std::abs(posx - chosenx) + std::abs(posy - choseny);
    return distanceNew < distanceCurrent;
}

int Bot::fakeTPF(int desty, int destx, const HexBoard &hb) {
    int profcount = 0;
    for (vector<int> vec : destx%2==1 ? neighborPosForOdd : neighborPosForEven) {
        int newy = desty + vec[1];
        int newx = destx + vec[0];
        if (newy < 0 || newy >= HexBoard::BOARD_SIZE || newx < 0 || newx >= HexBoard::BOARD_SIZE) continue;
        if (hb.board[newy][newx] == ' ') continue;
        if (hb.board[newy][newx] == '0') continue;
        if (hb.board[newy][newx] != StoneHelper::getName(this->getStone())) {
            profcount++;
        }
    }
    return profcount;
}

bool Bot::isBot() const {
    return true;
}