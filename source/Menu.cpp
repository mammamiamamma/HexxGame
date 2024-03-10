//
// Created by Ilya on 20.02.2024.
//

#include "Menu.h"
#include "Application.h"
#include "Game.h"
#include <iostream>
#include <fstream>

string totalPagesStr;
string selectedFile = "nofile";

//-------
bool isLoadedGame = false;
HexBoard hb;
Player p1;
Player p2;
Bot bot;
bool pvp = false;
Stone currStone;
//-------

namespace fs = std::filesystem;

Menu::Menu(Application& app)
        : app(app) {}

void Menu::initializeMainMenuButtons(vector<sf::RectangleShape>& buttArr, vector<sf::Text>& textArr) {
    sf::RectangleShape newGameButton = Application::createButton({500, 150});
    sf::RectangleShape loadGameButton = Application::createButton({500, 150});
    sf::RectangleShape exitButton = Application::createButton({500, 150});

    newGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2.f - newGameButton.getSize().x / 2.f), static_cast<float>(100)});
    loadGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - loadGameButton.getSize().x / 2), static_cast<float>(100 + app.WINDOW_SIZE_Y / 3)});
    exitButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - exitButton.getSize().x/2), static_cast<float>(100+2*app.WINDOW_SIZE_Y / 3)});

    buttArr.emplace_back(newGameButton);
    buttArr.emplace_back(loadGameButton);
    buttArr.emplace_back(exitButton);

    textArr.emplace_back(app.createText("NEW GAME", 50, newGameButton));
    textArr.emplace_back(app.createText("LOAD GAME", 50, loadGameButton));
    textArr.emplace_back(app.createText("EXIT", 50, exitButton));
}

void Menu::initializeNewGameButtons(vector<sf::RectangleShape>& buttArr, vector<sf::Text>& textArr) {
    sf::RectangleShape pvpButton = Application::createButton({500, 150});
    sf::RectangleShape pvbButton = Application::createButton({500, 150});
    sf::RectangleShape backButton = Application::createButton({500, 150});

    pvpButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvpButton.getSize().x / 2), static_cast<float>(100)});
    pvbButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvbButton.getSize().x / 2), static_cast<float>(100 + app.WINDOW_SIZE_Y / 3)});
    backButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - backButton.getSize().x / 2), static_cast<float>(100 + 2 * app.WINDOW_SIZE_Y / 3)});

    buttArr.emplace_back(pvpButton);
    buttArr.emplace_back(pvbButton);
    buttArr.emplace_back(backButton);

    textArr.emplace_back(app.createText("PL vs PL", 50, pvpButton));
    textArr.emplace_back(app.createText("PL vs BOT", 50, pvbButton));
    textArr.emplace_back(app.createText("BACK", 50, backButton));
}

int Menu::registerMouseClick(vector<sf::RectangleShape>& buttArr, sf::Vector2f mouse_pos){
    for (int i = 0; i < buttArr.size(); i++) {
        if (buttArr[i].getGlobalBounds().contains(mouse_pos)) {
            return i;
        }
    }
    return -1;
}

int Menu::registerMouseMove(vector<sf::RectangleShape>& buttArr, sf::Vector2f mouse_pos, int ind){
    bool isMouseOverButton = false; // Indicates if the mouse is over any button
    for (int i = 0; i < buttArr.size(); i++) {
        if (buttArr[i].getGlobalBounds().contains(mouse_pos)) {
            buttArr[i].setFillColor(sf::Color(100,100,100,192));
            ind = i;
            isMouseOverButton = true;
        }
    }
    if (!isMouseOverButton && ind!=-1) {
        buttArr[ind].setFillColor(sf::Color(0,0,0,192));
        ind = -1;
    }
    return ind;
}

int Menu::launchMenu() {
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;
    initializeMainMenuButtons(buttArr, textArr);
    restart:
    app.drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool shouldExit = false;
    sf::Event event{};
    while (app.window.isOpen() && !shouldExit) {
        while (app.window.pollEvent(event) && !shouldExit) {
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {
                    ind = registerMouseClick(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)));
                    if (ind!=-1){
                        shouldExit = true;
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    lastButtonIndex = registerMouseMove(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)), lastButtonIndex);
                    app.drawMenu(buttArr,textArr);
                }
                case sf::Event::Resized: {
                    app.drawMenu(buttArr, textArr);
                    break;
                }
                case sf::Event::Closed: return -1;
                default: break;
            }
        }
    }
    buttArr[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
    switch (ind){
        case 0:
            app.window.clear();
            switch (launchNewGameMenu()){ //i didnt want to go too deep into recursion so tried to do it this way, but maybe its not that bad.
                case -1: goto restart;
                case 0: return -1;
            }
            break;
        case 1:
            switch (launchLoadGameMenu()){
                case -1: {
                    ClearControlButtons();
                    ResetMenuState();
                    goto restart;
                } // restarting the main menu sequence
                case 1: {
                    ClearControlButtons();
                    ResetMenuState();
                    switch (loadGame()){
                        case 1:
                            playWithHuman();
                        case 2:
                            playWithBot();
                    }
                } // restarting the main menu sequence
            }
            break;
//        case 2:
//            return -1;
        default: return -1;
    }
    buttArr.clear();
    textArr.clear();
    return 0;
}

vector<string> Menu::getFileNames(){
    vector<string> filenames;
    string path = "saves/";
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) { // Check if it's a file (not a directory)
                string name = entry.path().filename().string();
                filenames.emplace_back(name);
            }
        }
    } catch (const fs::filesystem_error& err) {
        std::cerr << "Error accessing directory: " << err.what() << std::endl;
    }

    return filenames;
}

std::vector<std::string> Menu::split(const std::string& s) {
    std::vector<std::string> temp;
    std::istringstream tempstream(s);
    std::string part;
    while (std::getline(tempstream, part, '_')) {
        temp.emplace_back(part);
    }
    return temp;
}

vector<sf::Text> Menu::getTextForButton(const string& filename){
    vector<sf::Text> ret;
    string freeSpaces;
    string score1;
    string score2;
    string currentStone;
    string gamemode;
    ifstream inputFile("saves/"+filename);
    if (!inputFile) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return ret; // Return an error code
    }
    if (inputFile.is_open()) {
        string line;
        for (int i = 0; i<9; i++){
            std::getline(inputFile, line);
        }
        inputFile >> currentStone;
        inputFile >> freeSpaces;
        inputFile >> score1;
        inputFile >> score2;
        inputFile >> gamemode;
        inputFile.close();

        vector<string> parts = split(filename);
        parts[1].erase(parts[1].size()-4, parts[1].size());
        string s = "BLUE: "+score1;
        sf::Text score1Text = app.createText(s, 20);
        sf::Text score2Text = app.createText("RED: "+score2, 20);
        sf::Text freeSpaceText = app.createText("FREE SPACE: "+ freeSpaces, 20);
        sf::Text currTurnText = app.createText("CURRENT TURN: ", 20);
        sf::Text saveTimeText = app.createText("SAVED ON: "+parts[0]+", "+parts[1], 27);

        sf::Text stoneText = app.createText(currentStone, 20);
        stoneText.setOutlineThickness(2);
        if (currentStone == "B") {
            stoneText.setOutlineColor(sf::Color::Blue);
        } else {
            stoneText.setOutlineColor(sf::Color::Red);
        }

        score1Text.setOutlineColor(sf::Color::Blue);
        score1Text.setOutlineThickness(2);
        score2Text.setOutlineColor(sf::Color::Red);
        score2Text.setOutlineThickness(2);

        ret.emplace_back(saveTimeText);
        ret.emplace_back(score1Text);
        ret.emplace_back(score2Text);
        ret.emplace_back(currTurnText);
        ret.emplace_back(freeSpaceText);
        ret.emplace_back(stoneText);
    } else {
        std::cerr << "Error: Cannot open the file" << '\n';
    }
    return ret;
}

int Menu::launchLoadGameMenu(){
    vector<string> filenames = getFileNames();
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;

    sf::RectangleShape saveWindow = Application::createButton({740,870}, sf::Color::Cyan);
    saveWindow.setPosition({app.WINDOW_SIZE_X/2-saveWindow.getSize().x/2, 130});

    buttArr.emplace_back(saveWindow);
    for (int i = 0; i<filenames.size(); i++){
        sf::RectangleShape button = Application::createButton({700, 150});
        button.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - button.getSize().x / 2), static_cast<float>(150+ 170*(i%5))});
        buttArr.emplace_back(button);

        vector<sf::Text> buttonText = getTextForButton(filenames[i]);
        buttonText[0].setPosition({button.getPosition().x+10, button.getPosition().y+10});
        buttonText[1].setPosition({button.getPosition().x+10, button.getPosition().y+button.getSize().y/2-20});
        buttonText[2].setPosition({button.getPosition().x+200, button.getPosition().y+button.getSize().y/2-20});
        buttonText[3].setPosition({button.getPosition().x+10, button.getPosition().y+button.getSize().y/2+20});
        buttonText[5].setPosition({button.getPosition().x+255, button.getPosition().y+button.getSize().y/2+20});
        buttonText[4].setPosition({button.getPosition().x+400, button.getPosition().y+button.getSize().y/2-20});

        textArr.emplace_back(buttonText[0]);
        textArr.emplace_back(buttonText[1]);
        textArr.emplace_back(buttonText[2]);
        textArr.emplace_back(buttonText[3]);
        textArr.emplace_back(buttonText[5]);
        textArr.emplace_back(buttonText[4]);
    }

    sf::RectangleShape backButton = Application::createButton({300,100});
    backButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});
    sf::Text backText = app.createText("BACK", 40, backButton);

    controlButtons.emplace_back(backButton);
    controlButtonsText.emplace_back(backText);

    if (filenames.size()>5){
        isButtonNeeded = true;
        sf::RectangleShape prevButton = Application::createButton({300,100});
        sf::RectangleShape nextButton = Application::createButton({300,100});
        prevButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X/2-700), app.WINDOW_SIZE_Y/2-prevButton.getSize().y/2});
        nextButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X/2+400), app.WINDOW_SIZE_Y/2-prevButton.getSize().y/2});
        sf::Text prevButtonText = app.createText("PREV PAGE", 30, prevButton);
        sf::Text nextButtonText = app.createText( "NEXT PAGE", 30, nextButton);

        controlButtons.emplace_back(prevButton);
        controlButtons.emplace_back(nextButton);

        controlButtonsText.emplace_back(prevButtonText);
        controlButtonsText.emplace_back(nextButtonText);
    }
    int pageNum = 0;
    sf::Event event{};
    int totalPages = (int)filenames.size()/5;
    int lastButtonIndex = -1;
    bool isInControlButton = false;
    bool shouldExit = false;
    {
        stringstream ss;
        ss << totalPages+1;
        ss >> totalPagesStr;
    }
    app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, pageNum, totalPagesStr, isFirstPage,
            isLastPage,
            isButtonNeeded);
    while (app.window.isOpen() && !shouldExit){
        while (app.window.pollEvent(event) && !shouldExit){
            switch (event.type){
                case sf::Event::Closed:{
                    std::exit(1);
                }
                case sf::Event::MouseButtonPressed: {
                    int n = -1;
                    for (int i = 0; i<3; i++){
                        if (controlButtons[i].getGlobalBounds().contains(app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            n = i;
                        }
                    }
                    if (n!=-1){
                        if (n==0) return -1; // back to main menu
                        else if (n==1) pageNum--;
                        else pageNum++;
                        updatePageButtons(pageNum, totalPages);
                    }
                    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++){
                        if (buttArr[i].getGlobalBounds().contains(app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            selectedFile = filenames[i-1];
                            shouldExit = true;
                            break;
                        }
                    }
                    app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, pageNum, totalPagesStr, isFirstPage,
                                 isLastPage,
                                 isButtonNeeded);
                    break;
                }
                case sf::Event::MouseMoved: {
                    if (lastButtonIndex != -1) {
                        if (!isInControlButton && !buttArr[lastButtonIndex].getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0, 0, 0, 192));
                            lastButtonIndex = -1;
                        } else if (isInControlButton && !controlButtons[lastButtonIndex].getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            controlButtons[lastButtonIndex].setFillColor(sf::Color(0, 0, 0, 192));
                            isInControlButton = false;
                            lastButtonIndex = -1;
                        }
                    }
                    for (int i = 1 + pageNum * 5, j = i; i < j + 5 && i < buttArr.size(); i++) {
                        if (buttArr[i].getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            buttArr[i].setFillColor(sf::Color(100, 100, 100, 192));
                            lastButtonIndex = i;
                            goto skipisInButton;
                        }
                    }
                    for (int i = 0; i < controlButtons.size(); i++) {
                        if (controlButtons[i].getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            controlButtons[i].setFillColor(sf::Color(100, 100, 100, 192));
                            lastButtonIndex = i;
                            app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, pageNum, totalPagesStr,
                                         isFirstPage,
                                         isLastPage,
                                         isButtonNeeded);
                            isInControlButton = true;
                        }
                    }
                    skipisInButton:
                    app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, pageNum, totalPagesStr,
                                 isFirstPage,
                                 isLastPage,
                                 isButtonNeeded);
                    break;
                }
                default: break;
            }
        }
    }
    return 1;
}

void Menu::updatePageButtons(int pageNum, int totalPages) {
    if (pageNum<totalPages) {
        isLastPage = false;
    }
    else if (pageNum==totalPages){
        isLastPage = true;
        isFirstPage = false;
    }
    if (pageNum>0) {
        isFirstPage = false;
    } else if (pageNum==0) {
        isFirstPage = true;
        isLastPage = false;
    }
}

int Menu::launchNewGameMenu() {
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;

    initializeNewGameButtons(buttArr, textArr);

    app.drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool shouldExit = false;
    sf::Event event{};
    sf::Vector2f translated_pos;
    while (app.window.isOpen() && !shouldExit){
        while (app.window.pollEvent(event) && !shouldExit) {
            switch (event.type) {
                case sf::Event::MouseButtonPressed:{
                    ind = registerMouseClick(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)));
                    if (ind!=-1){
                        shouldExit = true;
                    }
                    break;
                }
                case sf::Event::Resized:{
                    app.drawMenu(buttArr, textArr);
                    break;
                }
                case sf::Event::Closed:{
                    shouldExit = true;
                    app.window.close();
                    break;
                }
                case sf::Event::MouseMoved: {
                    lastButtonIndex = registerMouseMove(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)), lastButtonIndex);
                    app.drawMenu(buttArr,textArr);
                }
                default: break;
            }
        }
    }
    buttArr.clear();
    switch (ind){
        case 0:
            return playWithHuman();
        case 1:
            return playWithBot();
        default:
            return -1;
    }
}

int Menu::playWithBot() {
    app.window.clear();
    app.window.display();
    app.window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    sf::RectangleShape exitButton = Application::createButton({300,100});
    exitButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});

    sf::Text exitText = app.createText( "BACK", 40, exitButton);

    vector<sf::RectangleShape> tempButton;
    vector<sf::Text> tempText;
    tempButton.emplace_back(exitButton);
    tempText.emplace_back(exitText);

    if (!isLoadedGame){
        hb = HexBoard();
        p1 = Player(Stone::BLUE);
        bot = Bot(Stone::RED);

        Game game(hb, p1, bot, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startGame();

        if (res == -1) return -1;
        else return 0;
    } else {
        Game game(hb, p1, bot, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startLoadedGame(currStone);

        if (res == -1) return -1;
        else return 0;
    }
}

int Menu::playWithHuman() {
    app.window.clear();
    app.window.display();
    app.window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    sf::RectangleShape exitButton = Application::createButton({300,100});
    exitButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});
    sf::Text exitText = app.createText("BACK", 40, exitButton);

    vector<sf::RectangleShape> tempButton;
    vector<sf::Text> tempText;
    tempButton.emplace_back(exitButton);
    tempText.emplace_back(exitText);

    if (!isLoadedGame){
        hb = HexBoard();
        p1 = Player(Stone::BLUE);
        p2 = Player(Stone::RED);

        Game game(hb, p1, p2, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startGame();

        if (res == -1) return -1;
        else return 0;
    } else {
        Game game(hb, p1, p2, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startLoadedGame(currStone);

        if (res == -1) return -1;
        else return 0;
    }
}

int Menu::loadGame() {
    if (selectedFile == "nofile") return -1; //check in case the load file wasnt chosen
    isLoadedGame = true;
    p1 = Player(Stone::BLUE);
    p2 = Player(Stone::RED);

    std::ifstream inputFile("saves/"+selectedFile);
    if (inputFile.is_open()) {
        for (int row = 0; row < HexBoard::BOARD_SIZE; ++row) {
            std::string line;
            if (!std::getline(inputFile, line)) {
                break;
            }
            for (size_t col = 0; col < HexBoard::BOARD_SIZE; ++col) {
                if (col >= line.length()) {
                    break;
                }
                switch (line[col]) {
                    case '0':
                        hb.board[row][col] = '0';
                        break;
                    case ' ':
                        hb.board[row][col] = ' ';
                        break;
                    case 'R':
                        hb.board[row][col] = 'R';
                        break;
                    case 'B':
                        hb.board[row][col] = 'B';
                        break;
                    default:
                        break;
                }
            }
        }
        string temp;
        inputFile >> temp;
        if (temp == "R") {
            currStone = Stone::RED;
        } else if (temp == "B") {
            currStone = Stone::BLUE;
        }
        int score;
        inputFile >> score;
        HexBoard::freeSpaces = score; //? why there??
        inputFile >> score;
        p1.setScore(score);
        inputFile >> score;
        p2.setScore(score);
        inputFile >> temp;
        if (temp == "Player") {
            pvp = true;
        } else {
            pvp = false;
        }
        inputFile.close();
    } else {
        std::cerr << "Error: Cannot open the file" << std::endl;
    }
    if (pvp) return 1;
    else return 2;
}

void Menu::ClearControlButtons() {
    controlButtons.clear();
    controlButtonsText.clear();
}

void Menu::ResetMenuState() {
    isFirstPage = true;
    isLastPage = false;
    isButtonNeeded = false;
}
