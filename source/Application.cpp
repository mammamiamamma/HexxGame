#include <map>
#include "Application.h"
#include "Game.h"
#include "Menu.h"

void Application::initiateShapeBoard(HexBoard& hb) {
    int rad = 50;
    int sideBound = 582; //545+37 mathematically BASED calculation to put the field in the center
    int topBound = 43/2; //also mathematical
    float linePadding;
    for (int i = 0; i < shapeBoard.size(); i++) {
        sf::Color col;
        for (int j = 0; j < shapeBoard[i].size(); j++) {
            shapeBoard[i][j] = sf::CircleShape(rad, 6);
            if (j % 2 == 0) {
                linePadding = rad;
            }
            else linePadding = 0;
            sf::Color tileColor = getTileColor(hb.board[i][j]);
            sf::Color borderColor = getBorderColor(hb.board[i][j]);
            shapeBoard[i][j].setPosition({sideBound + j * rad * 1.8f, static_cast<float>(topBound + (i*2.05)*rad+linePadding)});
            shapeBoard[i][j].rotate(sf::degrees(30));
            shapeBoard[i][j].setOutlineThickness(6.f);
            shapeBoard[i][j].setOutlineColor(borderColor);
            shapeBoard[i][j].setFillColor(tileColor);
        }
    }
    displayShapes();
}

void Application::updateShapeBoard(HexBoard& hb) {
    for (int i = 0; i < shapeBoard.size(); i++) {
        for (int j = 0; j < shapeBoard[i].size(); j++) {
            sf::Color tileColor = getTileColor(hb.board[i][j]);
            sf::Color borderColor = getBorderColor(hb.board[i][j]);
            shapeBoard[i][j].setOutlineColor(borderColor);
            shapeBoard[i][j].setFillColor(tileColor);
        }
    }
    displayShapes();
}

sf::Color Application::getTileColor(const char& tileContent) {
    auto it = colorMap.find(tileContent);
    return (it != colorMap.end()) ? it->second : sf::Color::Black;
}

sf::Color Application::getBorderColor(const char& tileContent) {
    auto it = borderMap.find(tileContent);
    return (it != borderMap.end()) ? it->second : sf::Color::Black;
}


bool Application::isClickedInsideTile(const sf::Vector2f point, const sf::CircleShape& tile) {
    float circleRadius = tile.getRadius();
    float vertSide = sqrt((2 * circleRadius) * (2 * circleRadius) - circleRadius * circleRadius), horizSide = circleRadius;

    sf::RectangleShape tileBoxY({ horizSide,  vertSide });
    sf::RectangleShape tileBoxX({ vertSide-10,  horizSide });
    tileBoxY.setPosition({ tile.getPosition().x - circleRadius * 0.13f, tile.getPosition().y + circleRadius * 0.53f });
    tileBoxX.setPosition({ tile.getPosition().x - circleRadius * 0.4f, tile.getPosition().y + circleRadius * 0.8f });

    return tileBoxX.getGlobalBounds().contains(point) || tileBoxY.getGlobalBounds().contains(point);
}

void Application::drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr) {
    window.clear();
    window.draw(background);
    for (const auto& button : buttArr) {
        window.draw(button);
    }
    for (const auto& text : textArr) {
        window.draw(text);
    }
    window.display();
}

void Application::drawMenu(
        const std::vector<sf::RectangleShape>& buttArr,
        const std::vector<sf::Text>& textArr,
        const std::vector<sf::RectangleShape>& controlButtons,
        const std::vector<sf::Text>& controlButtonsText,
        sf::Font& font,
        int& pageNum,
        string& totalPagesStr,
        bool isFirstPage,
        bool isLastPage,
        bool isButtonNeeded
        ) {
    stringstream ss;
    ss << pageNum+1;
    string pageStr;
    ss >> pageStr;
    window.clear();
    window.draw(background);
    window.draw(buttArr[0]);
    drawButton(controlButtons[0], controlButtonsText[0]);
    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++){
        window.draw(buttArr[i]);
    }
    for (int i = pageNum*30, j = i; i<j+30 && i<textArr.size(); i++){
        window.draw(textArr[i]);
    }

    if (isButtonNeeded) {
        if (!isLastPage) drawButton(controlButtons[2], controlButtonsText[2]);
        if (!isFirstPage) drawButton(controlButtons[1], controlButtonsText[1]);
    }
    sf::Text savefilestext(font, "SAVE FILES", 30);
    savefilestext.setPosition({static_cast<float>(window.getSize().x/2-255/2),80});
    window.draw(savefilestext);
    sf::Text pageIndexText(font, "PAGE "+pageStr + " OF " + totalPagesStr, 30);
    pageIndexText.setPosition({static_cast<float>(window.getSize().x/2-255/2),static_cast<float>(window.getSize().y-70)});
    window.draw(pageIndexText);
    window.display();
}

void Application::drawButton(const sf::RectangleShape& b, const sf::Text& text) {
    window.draw(b);
    window.draw(text);
}

void Application::displayShapes(){
    window.clear();
    window.draw(background);
    for (auto & i : shapeBoard)
    {
        for (int j = 0; j < shapeBoard[0].size(); j++)
        {
            window.draw(i[j]);
        }
    }
//    for (const auto & controlButton : controlButtons){
//        window.draw(controlButton);
//    }
//    for (const auto & i : controlButtonsText){
//        window.draw(i);
//    }
    window.display();
}

//bool Application::displayPossibleMoves(int posx, int posy, bool indicator) {
//    if (b.board[posy][posx] == ' ')
//        return false;
//    if (indicator) {
//        piecestolight.clear();
//        if (posx%2==1){
//            for (std::vector<int> vec : possibleMovesForOdd) {
//                int newx = posx + vec[0];
//                int newy = posy + vec[1];
//                if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
//                    continue;
//                if (b.board[newy][newx] == '0') {
//                    piecestolight.push_back({ newx, newy });
//                }
//            }
//        } else {
//            for (std::vector<int> vec : possibleMovesForEven) {
//                int newx = posx + vec[0];
//                int newy = posy + vec[1];
//                if (newx < 0 || newx >= HexBoard::BOARD_SIZE || newy < 0 || newy >= HexBoard::BOARD_SIZE)
//                    continue;
//                if (b.board[newy][newx] == '0') {
//                    piecestolight.push_back({ newx, newy });
//                }
//            }
//        }
//
//        if (piecestolight.empty()) return false;
//
//        sf::Color color = getTileColor(b.board[posy][posx]);
//        for (std::vector<int> vec : piecestolight) {
//            int newx = vec[0];
//            int newy = vec[1];
//            shapeBoard[newy][newx].setOutlineColor(color);
//        }
//    }
//    else {
//        for (std::vector<int> vec : piecestolight) {
//            shapeBoard[vec[1]][vec[0]].setOutlineColor(getBorderColor(b.board[vec[1]][vec[0]]));
//        }
//        piecestolight.clear();
//    }
//    displayShapes();
//    return true;
//}

void Application::displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb, int posx, int posy) {
    for (std::vector<int> vec : piecesToLight) {
        shapeBoard[vec[1]][vec[0]].setOutlineColor(getTileColor(hb.board[posy][posx]));
    }
    displayShapes();
}
void Application::displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb) {
    for (std::vector<int> vec : piecesToLight) {
        shapeBoard[vec[1]][vec[0]].setOutlineColor(getBorderColor(hb.board[vec[1]][vec[0]]));
    }
    displayShapes();
}


