//
// Created by Ilya on 30.05.2023.
//

#include "Game.h"
#include "Bot.h"
#include <utility>
#include <fstream>
#include <filesystem>
#include <chrono>

Game::Game(HexBoard& b1, Player& p1, Player& p2, Application& app) : b(b1), p1(p1), p2(p2), app(app), bot(nullptr) { //maybe change back
    this->currentStone = p1.getStone();
    this->otherStone = p2.getStone();
    if (p2.isBot()) {
        bot = dynamic_cast<Bot*>(&this->p2);
    }
}

void Game::setControlButtons(vector<Button>& vec){
    this->controlButtons = vec;
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

int Game::registerClickOnTile(sf::Vector2f mouse_pos){
    static int originalHexX = -1;
    static int originalHexY = -1;
    int posx;
    int posy;
    getPlayerFromStone(currentStone).createPossibleMoves(b);
    if (getPlayerFromStone(currentStone).positionsandmoves.empty()) return -1;
    else {
        getPlayerFromStone(currentStone).positions.clear();
        getPlayerFromStone(currentStone).positionsandmoves.clear();
    }
    bool found = false;
    for (int i = 0; i < 9 && !found; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (Application::isClickedInsideTile(mouse_pos, app.shapeBoard[i][j])){
                posx = j;
                posy = i;
                found = true;
                break;
            }
        }
    }
    if (!found) return 0; //return 0 if click isnt registered as a click on tile
    if (originalHexX == -1 && originalHexY == -1) {
        if (
                b.board[posy][posx] == '0' ||
                b.board[posy][posx] == StoneHelper::getName(otherStone) ||
                b.board[posy][posx] == ' '
                )
            return 0;
        std::vector<std::vector<int>> piecestolight = getPlayerFromStone(currentStone).createPossibleMoves( b, posx, posy);
        app.displayPossibleMoves(piecestolight, b, posx, posy); //maybe include posx and posy in the array somehow to not pass them onto the method?
        app.displayShapes(controlButtons);
        originalHexX = posx;
        originalHexY = posy;
    } else {
        if (
                (originalHexX == posx && originalHexY == posy) ||
                (b.board[posy][posx] == StoneHelper::getName(currentStone)) ||
                (b.board[posy][posx] == StoneHelper::getName(otherStone)) ||
                (b.board[posy][posx] == ' ')
                )
        {
            std::vector<std::vector<int>> piecestolight = getPlayerFromStone(currentStone).createPossibleMoves( b, originalHexX, originalHexY);
            app.displayPossibleMoves(piecestolight, b);
            app.displayShapes(controlButtons);
            originalHexX = -1;
            originalHexY = -1;
            return 0;
        }
        if (b.makeAMove(originalHexY, originalHexX, posy, posx, getPlayerFromStone(currentStone), getPlayerFromStone(otherStone))) {
            std::swap(currentStone, otherStone);
            app.updateShapeBoard(b);
            app.displayShapes(controlButtons);
            if (!b.isGameValid()){
                sf::sleep(sf::milliseconds(1000));
                return -1;
            }
        } else {
            std::cout << "Invalid move. Please try again." << std::endl;
            return 0;
        }

        originalHexX = -1;
        originalHexY = -1;

        if (p2.isBot()){
            cout << "Bot's move" << endl;
            vector<int> move = bot->makeMove(b);
            if (b.makeAMove(move[1], move[0], move[3], move[2], getPlayerFromStone(currentStone), getPlayerFromStone(otherStone))) {
                cout << "Bot made a move: " << move[1] << " " << move[0] << " to " << move[3] << " " << move[2] << endl;
                std::swap(currentStone, otherStone);
                app.updateShapeBoard(b);
                sf::sleep(sf::milliseconds(1000)); //purely so that its more comfortable to track moves
                app.displayShapes(controlButtons);
                if (!b.isGameValid()){
                    sf::sleep(sf::milliseconds(1000));
                    return -1;
                }
            } else {
                std::cout << "Invalid move. Please try again." << std::endl;
            }
        }
    }
    return 0;
}
void Game::saveGame(){
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(now, "%Y-%m-%d_%H-%M-%S");
    std::string timeStr = oss.str();
    filename = timeStr+".txt";

    std::ofstream outputFile("saves/"+filename);  // Convert path to string
    if (!outputFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return; // Return an error code
    }
    if (outputFile.is_open()) {
        if (p2.isBot()) {
            outputFile << "Bot" << '\n';
        } else {
            outputFile << "Player" << '\n';
        }
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
        if (p2.isBot()) {
            outputFile << bot->getScore() << '\n';
        } else {
            outputFile << p2.getScore() << '\n';
        }
        outputFile.close();
        cout << "Game saved to "<< "saves/"+filename << endl;
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

int Game::registerClickOnButtons(sf::Vector2f mouse_pos){
    int ind = -1;
    for (int i = 0; i<controlButtons.size(); i++){
        if (controlButtons[i].getShape().getGlobalBounds().contains(mouse_pos)){
            ind = i;
            break;
        }
    }
    if (ind==-1) return 0; //nothing registered i guess
    else {
        switch (ind) {
            case 0: {
                return -1; // code for exiting the game (to main menu)
            }
            case 1: {
                return 2; // code for saving the game (not implemented yet)
            }
            default: return 0; //just in case
        }
    }
}

void Game::startGame() {
    putDefaultStones();

    sf::Event event{};
    app.initiateShapeBoard(b);
    app.displayShapes(controlButtons);
    bool shouldExit = false;
    while (app.window.isOpen() && !shouldExit) {
        while (HexBoard::freeSpaces > 0 && p1.getScore() > 0 && p2.getScore() > 0 && !shouldExit) {
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed: {
                        auto translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        if (event.mouseButton.button != sf::Mouse::Button::Left) {
                            continue;
                        }
                        if (registerClickOnTile(translated_pos) == -1){
                            shouldExit = true;
                            break;
                        } else {
                            switch (registerClickOnButtons(translated_pos)) {
                                case 0: continue;
                                case -1: {
                                    shouldExit = true;
                                    break;
                                }
                                case 2: {
                                    saveGame(); //save the game?
                                    shouldExit = true;
                                }
                            }
                        }
                        break;
                    }
                    case sf::Event::KeyPressed:
                        registerKeyPressed(event.key);
                        break;
                    case sf::Event::Resized:
                        app.displayShapes(controlButtons);
                        break;
                    case sf::Event::Closed:
                        app.window.close(); // if i want to exit then i need to return something, if i want to go to main menu i should comment this. What do?
                        shouldExit = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
//    finish:
    declareWinner();
}

void Game::startLoadedGame(Stone currStone){
    app.initiateShapeBoard(b);
    app.displayShapes(controlButtons);
    currentStone = currStone;
    if (currentStone == Stone::RED){
        otherStone = Stone::BLUE;
    } else {
        otherStone = Stone::RED;
    }
    bool shouldExit = false;
    app.displayShapes(controlButtons);
    sf::Event event{};
    while (app.window.isOpen() && !shouldExit) {
        while (HexBoard::freeSpaces > 0 && p1.getScore() > 0 && p2.getScore() > 0 && !shouldExit) {
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed: {
                        if (event.mouseButton.button != sf::Mouse::Button::Left) {
                            continue;
                        }
                        auto translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        if (registerClickOnTile(translated_pos) == -1) {
                            shouldExit = true;
                            break;
                        } else {
                            switch (registerClickOnButtons(translated_pos)) {
                                case 0: continue;
                                case -1: {
                                    shouldExit = true;
                                    break;
                                }
                                case 2: {
                                    saveGame(); //save the game?
                                    shouldExit = true;
                                }
                            }
                        }
                        break;
                    }
                    case sf::Event::KeyPressed:
                        registerKeyPressed(event.key);
                        break;
                    case sf::Event::Resized:
                        app.displayShapes(controlButtons);
                        break;
                    case sf::Event::Closed:
                        app.window.close();
                        shouldExit = true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    declareWinner();
}


