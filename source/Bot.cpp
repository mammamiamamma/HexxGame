#include "Bot.h"
#include "HexBoard.h"
#include <random>

Bot::Bot() : Player() {}

Bot::Bot(Stone stone) : Player(stone) {}

std::vector<int> Bot::makeMove(HexBoard &hb) {
    std::vector<int> move = {0, 0, 0, 0};
    findallpositions(hb);
    createPossibleMoves(hb);
    move = evaluatemoves(hb);
    positions.clear();
    positionsandmoves.clear();

    return move;
}

std::vector<int> Bot::evaluatemoves(HexBoard &hb) {
    std::vector<int> chosenmove;
    int maxprof = 0;
    int origposx = 0;
    int origposy = 0;
    int chosenx = 0;
    int choseny = 0;
    for (auto& pos : positionsandmoves) {
        int posx = pos[0].first;
        int posy = pos[0].second;
        if (hb.board[pos[1].second][pos[1].first] == '0') {
            int count = fakeTPF(pos[1].second, pos[1].first, *this, hb);
            if (count > maxprof) {
                origposx = posx;
                origposy = posy;
                chosenx = pos[1].first;
                choseny = pos[1].second;
                maxprof = count;
            }
            else if (count == maxprof && (abs(posx-chosenx)<abs(origposx-chosenx) || abs(posy-choseny)<abs(origposy-choseny))){
                origposx = posx;
                origposy = posy;
                chosenx = pos[1].first;
                choseny = pos[1].second;
            }
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
//            if (origposx%2==0){
//                while (
//                        (abs(origposx-chosenx)==1 && origposy-choseny==1) ||
//                        std::abs(origposx - chosenx) >= 2 ||
//                        std::abs(origposy - choseny) >= 2)
//                {
//                    randomIndex = dis(gen);
//                    origposx = positionsandmoves[randomIndex][0].first;
//                    origposy = positionsandmoves[randomIndex][0].second;
//                    chosenx = positionsandmoves[randomIndex][1].first;
//                    choseny = positionsandmoves[randomIndex][1].second;
//                }
//            }
//            else {
//                while (
//                        (abs(origposx-chosenx)==1 && origposy-choseny==-1) ||
//                        std::abs(origposx - chosenx) >= 2 ||
//                        std::abs(origposy - choseny) >= 2)
//                {
//                    randomIndex = dis(gen);
//                    origposx = positionsandmoves[randomIndex][0].first;
//                    origposy = positionsandmoves[randomIndex][0].second;
//                    chosenx = positionsandmoves[randomIndex][1].first;
//                    choseny = positionsandmoves[randomIndex][1].second;
//                }
//            }
        }
    }
    chosenmove.push_back(origposx);
    chosenmove.push_back(origposy);
    chosenmove.push_back(chosenx);
    chosenmove.push_back(choseny);
    return chosenmove;
}


int Bot::fakeTPF(int desty, int destx, Player& p, HexBoard &hb) {
    int profcount = 0;
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
    for (int i = 0; i < 6; i++) {
        int newy = desty + neighborPos[i][1];
        int newx = destx + neighborPos[i][0];
        if (newy < 0 || newy >= HexBoard::BOARD_SIZE || newx < 0 || newx >= HexBoard::BOARD_SIZE) continue;
        if (hb.board[newy][newx] == ' ') continue;
        if (hb.board[newy][newx] == '0') continue;
        if (hb.board[newy][newx] != StoneHelper::getName(p.getStone())) {
            profcount++;
        }
    }
    return profcount;
}

bool Bot::isBot() const {
    return true;
}