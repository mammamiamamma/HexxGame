//
// Created by Ilya on 30.05.2023.
//

#include "Game.h"
#include "Bot.h"
#include <utility>
#include <fstream>
#include <filesystem>
#include <chrono>

int posx;
int posy;
int destx;
int desty;
int originalHexX = 0;
int originalHexY = 0;
bool isOriginalHexSet = false;
bool isBot;

Game::Game(HexBoard& b1, Player& p1, Player& p2, Application& app) : b(b1), p1(p1), p2(p2), app(app), bot(nullptr) { //maybe change back
    this->currentStone = p1.getStone();
    this->otherStone = p2.getStone();
    if (p2.isBot()) {
        isBot = true;
        bot = dynamic_cast<Bot*>(&this->p2);
    }
}

void Game::putDefaultStones() {
    b.putStone(0, 4, p1);
    b.putStone(2, 0, p2);
    b.putStone(2, 8, p2);
    b.putStone(6, 0, p1);
    b.putStone(6, 8, p1);
    b.putStone(8, 4, p2);
}

Player& Game::getPlayerFromStone(Stone stone) {
    if (stone == Stone::RED) {
        return p2;
    } else if (stone == Stone::BLUE) {
        return p1;
    }
    return p1;
}

int Game::registerClickOnTile(sf::Event::MouseButtonEvent mouseButtonEvent){
    if (mouseButtonEvent.button != sf::Mouse::Button::Left) {
        return 0;
    }
    getPlayerFromStone(currentStone).createPossibleMoves(b);
    if (getPlayerFromStone(currentStone).positionsandmoves.empty()) return -1;
    else {
        getPlayerFromStone(currentStone).positionsandmoves.clear();
    }
    auto mouse_pos = sf::Mouse::getPosition(app.window);
    auto translated_pos = app.window.mapPixelToCoords(mouse_pos);
    bool found = false;
    for (int i = 0; i < 9 && !found; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (Application::isClickedInsideTile(translated_pos, app.shapeBoard[i][j])){
                posx = j;
                posy = i;
                found = true;
                break;
            }
        }
    }
    if (!isOriginalHexSet) {
        if (
                b.board[posy][posx] == '0' ||
                b.board[posy][posx] == StoneHelper::getName(otherStone) ||
                b.board[posy][posx] == ' '
                )
            return 0;
        if (!app.displayPossibleMoves(posx, posy, true)) return 0;
        originalHexX = posx;
        originalHexY = posy;
        isOriginalHexSet = true;
    } else {
        if (
                (originalHexX == posx && originalHexY == posy) ||
                (b.board[posy][posx] == StoneHelper::getName(currentStone)) ||
                (b.board[posy][posx] == StoneHelper::getName(otherStone)) ||
                (b.board[posy][posx] == ' ')
                )
        {
            app.displayPossibleMoves(originalHexX, originalHexY, false);
            originalHexX = -1;
            originalHexY = -1;
            posx = -1;
            posy = -1;
            isOriginalHexSet = false;
            return 0;
        }
        destx = posx;
        desty = posy;
        getPlayerFromStone(currentStone);
        getPlayerFromStone(otherStone);
        if (b.makeAMove(originalHexY, originalHexX, desty, destx, getPlayerFromStone(currentStone), getPlayerFromStone(otherStone))) {
            std::swap(currentStone, otherStone);
            app.initiateShapeBoard(b);
        } else {
            std::cout << "Invalid move. Please try again." << std::endl;
            return 0;
        }
        isOriginalHexSet = false;
        app.displayPossibleMoves(originalHexX, originalHexY, false);

        if (isBot){
            sf::sleep(sf::milliseconds(1000));
            cout << "Bot's move" << endl;
            vector<int> move = bot->makeMove(b);
            if (b.makeAMove(move[1], move[0], move[3], move[2], getPlayerFromStone(currentStone), getPlayerFromStone(otherStone))) {
                cout << "Bot made a move: " << move[1] << " " << move[0] << " to " << move[3] << " " << move[2] << endl;
                std::swap(currentStone, otherStone);
                app.initiateShapeBoard(b);
            } else {
                std::cout << "Invalid move. Please try again." << std::endl;
            }
        }

    }
}

void Game::saveGame(){
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(now, "%Y-%m-%d_%H-%M-%S");
    std::string timeStr = oss.str();
    filename = timeStr+".txt";

    std::ofstream outputFile(filePath+filename);  // Convert path to string
    if (!outputFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return; // Return an error code
    }
    if (outputFile.is_open()) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                switch(b.board[i][j]) {
                    case ' ':
                        outputFile << " ";
                        break;
                    case '0':
                        outputFile << "0";
                        break;
                    case 'R':
                        outputFile << "R";
                        break;
                    case 'B':
                        outputFile << "B";
                        break;
                }
            }
            outputFile << '\n';
        }
        outputFile << StoneHelper::getName(currentStone) << '\n';
        outputFile << HexBoard::freeSpaces << '\n';
        outputFile << p1.getScore() << '\n';
        if (isBot) {
            outputFile << bot->getScore() << '\n';
            outputFile << "Bot" << '\n';
        } else {
            outputFile << p2.getScore() << '\n';
            outputFile << "Player" << '\n';
        }
        outputFile.close();
        cout << "Game saved to "<< filePath+filename << endl;
    } else {
        std::cerr << "Error: Cannot open the file" << '\n';
    }
}

void Game::registerKeyPressed(sf::Event::KeyEvent keyEvent){
    if (keyEvent.code == sf::Keyboard::Key::S) {
        saveGame();
    }
}

void Game::declareWinner(){
    cout << "Game over!" << endl;
    if (p1.getScore() > p2.getScore()) {
        cout << "Player 1 (B) won!" << endl;
    } else if (p1.getScore() < p2.getScore()) {
        cout << "Player 2 (R) won!" << endl;
    } else {
        cout << "Draw!" << endl;
    }
}

void Game::startGame() {
    putDefaultStones();

    sf::Event event{};
    app.initiateShapeBoard(b);

    while (app.window.isOpen()) {
        while (HexBoard::freeSpaces > 0 && p1.getScore() > 0 && p2.getScore() > 0) {
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        if (registerClickOnTile(event.mouseButton)==-1){
                            goto finish;
                        }
                        break;
                    case sf::Event::KeyPressed:
                        registerKeyPressed(event.key);
                        break;
                    case sf::Event::Resized:
                        app.displayShapes();
                        break;
                    case sf::Event::Closed:
                        app.window.close();
                        goto finish;
                    default:
                        break;
                }
            }
        }
        finish:
        declareWinner();
        std::exit(0); //change it maybe
    }
}

void Game::startLoadedGame(Stone currStone){
    app.initiateShapeBoard(b);
    currentStone = currStone;
    if (currentStone == Stone::RED){
        otherStone = Stone::BLUE;
    } else {
        otherStone = Stone::RED;
    }
    app.displayShapes();
    sf::Event event{};
    while (app.window.isOpen()) {
        while (HexBoard::freeSpaces > 0 && p1.getScore() > 0 && p2.getScore() > 0) {
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        if (registerClickOnTile(event.mouseButton)==-1){
                            goto finish;
                        }
                        break;
                    case sf::Event::KeyPressed:
                        registerKeyPressed(event.key);
                        break;
                    case sf::Event::Resized:
                        app.displayShapes();
                        break;
                    case sf::Event::Closed:
                        app.window.close();
                        goto finish;
                    default:
                        break;
                }
            }
        }
        finish:
        declareWinner();
        std::exit(0); //change it maybe
    }
}


