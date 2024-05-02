#include "Application.h"
#include "Game.h"

void Application::changeState() {
    if (isFullscreen) {
        isFullscreen = false;
        window.create(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Default, sf::State::Windowed, windowSettings);
    }
    else {
        isFullscreen = true;
        window.create(sf::VideoMode({1920, 1080}), "Hexx Game", sf::Style::Default, sf::State::Fullscreen, windowSettings);
    }
}

void Application::initiateShapeBoard(HexBoard& hb) {
    float rad = 50;
    float sideBound = 582; //545+37 mathematically BASED calculation to put the field in the center
    float topBound = 21.5; //also mathematical
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

void Application::drawButton(Button& button) {
    window.draw(button.getShape());
    for (const auto& text : button.getText()) {
        window.draw(text);
    }
}

//void Application::drawButton(vector<Button>& buttonArr) {
//    for (auto& obj : buttonArr) {
//        window.draw(obj.getShape());
//        for (const auto& text : obj.getText()) {
//            window.draw(text);
//        }
//    }
//}

void Application::drawMenu(vector<Button>& buttonArr) {
    window.clear();
    window.draw(background);
    for (auto& obj : buttonArr) {
        window.draw(obj.getShape());
        for (const auto& text : obj.getText()) {
            window.draw(text);
        }
    }
    window.display();
}

sf::Text Application::createText(const string &textContent, unsigned int characterSize) const {
    sf::Text text(font, textContent, characterSize);
    return text;
}

void Application::drawMenu(
        std::vector<Button>& buttArr,
        std::vector<Button>& controlButtons,
        int& pageNum,
        string& totalPagesStr,
        bool isFirstPage,
        bool isLastPage,
        bool isButtonNeeded
        ) {
    string pageStr;
    {
        stringstream ss;
        ss << pageNum+1;
        ss >> pageStr;
    }
    window.clear();
    window.draw(background);
    window.draw(buttArr[0].getShape());
    drawButton(controlButtons[0]);
    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++){
        drawButton(buttArr[i]);
    }
    if (isButtonNeeded) {
        if (!isLastPage) drawButton(controlButtons[2]);
        if (!isFirstPage) drawButton(controlButtons[1]);
    }
    sf::Text savefilestext(font, "SAVE FILES", 30); //should not be created each time the window is drawn, obviously
    savefilestext.setPosition({static_cast<float>(WINDOW_SIZE_X/2-127.5),80});
    window.draw(savefilestext);
    sf::Text pageIndexText(font, "PAGE "+pageStr + " OF " + totalPagesStr, 30);
    pageIndexText.setPosition({static_cast<float>(WINDOW_SIZE_X/2-127.5),static_cast<float>(WINDOW_SIZE_Y-70)});
    window.draw(pageIndexText);
    window.display();
}

void Application::displayShapes(vector<Button>& buttArr){
    window.clear();
    window.draw(background);
    for (auto& butt : buttArr){
        drawButton(butt);
    }
    for (auto & i : shapeBoard)
    {
        for (int j = 0; j < shapeBoard[0].size(); j++)
        {
            window.draw(i[j]);
        }
    }
    window.display();
}

void Application::displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb, int posx, int posy) {
    for (std::vector<int> vec : piecesToLight) {
        shapeBoard[vec[1]][vec[0]].setOutlineColor(getTileColor(hb.board[posy][posx]));
    }
}
void Application::displayPossibleMoves(std::vector<std::vector<int>>& piecesToLight, HexBoard& hb) {
    for (std::vector<int> vec : piecesToLight) {
        shapeBoard[vec[1]][vec[0]].setOutlineColor(getBorderColor(hb.board[vec[1]][vec[0]]));
    }
}


