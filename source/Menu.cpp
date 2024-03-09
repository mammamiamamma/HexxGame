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

Menu::Menu(sf::Font& font, Application& app)
        : font(font), app(app) {}

void Menu::runMenu() {
    if (launchMenu() == -1) {
        app.window.close();
    }
}

int Menu::launchMenu() {
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;
    sf::RectangleShape newGameButton = app.createButton({500, 150});
    sf::RectangleShape loadGameButton = app.createButton({500, 150});
    sf::RectangleShape exitButton = app.createButton({500, 150});

    newGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2.f - newGameButton.getSize().x / 2.f), static_cast<float>(100)});
    loadGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - loadGameButton.getSize().x / 2), static_cast<float>(100 + app.WINDOW_SIZE_Y / 3)});
    exitButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - exitButton.getSize().x/2), static_cast<float>(100+2*app.WINDOW_SIZE_Y / 3)});

    sf::Text newGameText = app.createText(font, "NEW GAME", 50, newGameButton);
    sf::Text loadGameText = app.createText(font, "LOAD GAME", 50, loadGameButton);
    sf::Text exitText = app.createText(font, "EXIT", 50, exitButton);

    buttArr.emplace_back(newGameButton);
    buttArr.emplace_back(loadGameButton);
    buttArr.emplace_back(exitButton);

    textArr.emplace_back(newGameText);
    textArr.emplace_back(loadGameText);
    textArr.emplace_back(exitText);

    restart:
    app.drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool isInButton;
    {
        sf::Event event{};
        sf::Vector2f translated_pos;
        while (app.window.isOpen()) {
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                ind = i;
                                goto finish;
                            }
                        }
                        break;
                    case sf::Event::Resized:
                        app.drawMenu(buttArr, textArr);
                        break;
                    case sf::Event::Closed:
                        return -1;
                    case sf::Event::MouseMoved:
                        translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                buttArr[i].setFillColor(sf::Color(100,100,100,192));
                                lastButtonIndex = i;
                                isInButton = true;
                                app.drawMenu(buttArr, textArr);
                                goto skipisInButton;
                            }
                        }
                        isInButton = false;
                    skipisInButton:
                        if (!isInButton && lastButtonIndex!=-1) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
                            lastButtonIndex = -1;
                            app.drawMenu(buttArr, textArr);
                        }
                    default:
                        break;
                }
            }
        }
    }
    finish:
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
                    controlButtons.clear();
                    controlButtonsText.clear();
                    isFirstPage = true;
                    isLastPage = false;
                    isButtonNeeded = false;
                    goto restart;
                } // restarting the main menu sequence
                case 1: {
                    controlButtons.clear();
                    controlButtonsText.clear();
                    isFirstPage = true;
                    isLastPage = false;
                    isButtonNeeded = false;
                    switch (loadGame()){
                        case 1: playwithhuman();
                        case 2: playwithbot();
                    }
                } // restarting the main menu sequence
            }
            break;
        case 2:
            return -1;
    }
    buttArr.clear();
    return 0;
}

vector<string> Menu::getFileNames(){
    vector<string> filenames;
    string path = "saves/";
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) { // Check if it's a file (not a directory)
                string name = entry.path().filename().string();
                filenames.push_back(name);
            }
        }
    } catch (const fs::filesystem_error& err) {
        std::cerr << "Error accessing directory: " << err.what() << std::endl;
    }

    return filenames;
}

std::vector<std::string> Menu::split(const std::string& s, char delimiter) {
    std::vector<std::string> temp;
    std::istringstream tempstream(s);
    std::string part;
    while (std::getline(tempstream, part, delimiter)) {
        temp.push_back(part);
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

        vector<string> parts = split(filename, '_');
        parts[1].erase(parts[1].size()-4, parts[1].size());
        string s = "BLUE: "+score1;
        sf::Text score1Text(font, s, 20);
        sf::Text score2Text(font, "RED: "+score2, 20);
        sf::Text freeSpaceText(font, "FREE SPACE: "+ freeSpaces, 20);
        sf::Text currTurnText(font, "CURRENT TURN: ", 20);
        sf::Text saveTimeText(font, "SAVED ON: "+parts[0]+", "+parts[1], 27);

        sf::Text stoneText(font, currentStone, 20);
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

        ret.push_back(saveTimeText);
        ret.push_back(score1Text);
        ret.push_back(score2Text);
        ret.push_back(currTurnText);
        ret.push_back(freeSpaceText);
        ret.push_back(stoneText);

    } else {
        std::cerr << "Error: Cannot open the file" << '\n';
    }
    return ret;
}

int Menu::launchLoadGameMenu(){
    vector<string> filenames = getFileNames();
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;

    sf::RectangleShape saveWindow = app.createButton({740,870}, sf::Color::Cyan);
    saveWindow.setPosition({app.WINDOW_SIZE_X/2-saveWindow.getSize().x/2, 130});

    buttArr.emplace_back(saveWindow);
    for (int i = 0; i<filenames.size(); i++){
        sf::RectangleShape button = app.createButton({700, 150});
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

    sf::RectangleShape backButton = app.createButton({300,100});
    backButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});
    sf::Text backText = app.createText(font, "BACK", 40, backButton);

    controlButtons.emplace_back(backButton);
    controlButtonsText.emplace_back(backText);

    if (filenames.size()>5){
        isButtonNeeded = true;
        sf::RectangleShape prevButton = app.createButton({300,100});
        sf::RectangleShape nextButton = app.createButton({300,100});
        prevButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X/2-700), app.WINDOW_SIZE_Y/2-prevButton.getSize().y/2});
        nextButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X/2+400), app.WINDOW_SIZE_Y/2-prevButton.getSize().y/2});
        sf::Text prevButtonText = app.createText(font, "PREV PAGE", 30, prevButton);
        sf::Text nextButtonText = app.createText(font, "NEXT PAGE", 30, nextButton);

        controlButtons.emplace_back(prevButton);
        controlButtons.emplace_back(nextButton);

        controlButtonsText.emplace_back(prevButtonText);
        controlButtonsText.emplace_back(nextButtonText);
    }
    int pageNum = 0;
    sf::Event event{};
    sf::Vector2f translated_pos;
    int totalPages = filenames.size()/5;
    int lastButtonIndex = -1;
    bool isInControlButton = false;
    {
        stringstream ss;
        ss << totalPages+1;
        ss >> totalPagesStr;
    }
    app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, font, pageNum, totalPagesStr, isFirstPage,
            isLastPage,
            isButtonNeeded);
    while (app.window.isOpen()){
        while (app.window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:{
                    std::exit(1);
                }
                case sf::Event::MouseButtonPressed:{
                    translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++){
                        if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                            selectedFile = filenames[i-1];
                            goto loadedGameChosen;
                        }
                    }
                    for (int i = 0; i<3; i++){
                        if (controlButtons[i].getGlobalBounds().contains(translated_pos)) {
                            if (i==0) return -1; // back to main menu
                            if (i==1) {
                                pageNum--;
                            } else {
                                pageNum++;
                            }
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
                            app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, font, pageNum, totalPagesStr, isFirstPage,
                                         isLastPage,
                                         isButtonNeeded);
                            goto skip;
                        }
                    }
                    skip:
                    break;
                }
                case sf::Event::MouseMoved:
                    translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                    if (lastButtonIndex!=-1){
                        if (!isInControlButton && !buttArr[lastButtonIndex].getGlobalBounds().contains(translated_pos)) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0, 0, 0, 192));
                            lastButtonIndex = -1;
                        }
                        else if (isInControlButton && !controlButtons[lastButtonIndex].getGlobalBounds().contains(translated_pos)){
                            controlButtons[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
                            isInControlButton = false;
                            lastButtonIndex = -1;
                        }
                    }
                    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++) {
                        if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                            buttArr[i].setFillColor(sf::Color(100,100,100,192));
                            lastButtonIndex = i;
                            goto skipisInButton;
                        }
                    }
                    for (int i = 0; i<controlButtons.size(); i++){
                        if (controlButtons[i].getGlobalBounds().contains(translated_pos)) {
                            controlButtons[i].setFillColor(sf::Color(100,100,100,192));
                            lastButtonIndex = i;
                            app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, font, pageNum, totalPagesStr, isFirstPage,
                                         isLastPage,
                                         isButtonNeeded);
                            isInControlButton = true;
                        }
                    }
                    skipisInButton:
                    app.drawMenu(buttArr, textArr, controlButtons, controlButtonsText, font, pageNum, totalPagesStr, isFirstPage,
                                 isLastPage,
                                 isButtonNeeded);
                    break;
                default: {
                    break;
                }
            }
        }
    }
    loadedGameChosen:

    return 1;
}

int Menu::launchNewGameMenu() {
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;
    sf::RectangleShape pvpButton = app.createButton({500, 150});
    sf::RectangleShape pvbButton = app.createButton({500, 150});
    sf::RectangleShape backButton = app.createButton({500, 150});
    pvpButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvpButton.getSize().x / 2), static_cast<float>(100)});
    pvbButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvbButton.getSize().x / 2), static_cast<float>(100 + app.WINDOW_SIZE_Y / 3)});
    backButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - backButton.getSize().x / 2), static_cast<float>(100 + 2 * app.WINDOW_SIZE_Y / 3)});

    sf::Text pvpText = app.createText(font, "PL vs PL", 50, pvpButton);
    sf::Text pvbText = app.createText(font, "PL vs BOT", 50, pvbButton);
    sf::Text backText = app.createText(font, "BACK", 50, backButton);

    buttArr.emplace_back(pvpButton);
    buttArr.emplace_back(pvbButton);
    buttArr.emplace_back(backButton);

    textArr.emplace_back(pvpText);
    textArr.emplace_back(pvbText);
    textArr.emplace_back(backText);

    app.drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool isInButton;
    {
        sf::Event event{};
        sf::Vector2f translated_pos;
        while (app.window.isOpen()){
            while (app.window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        for (int i = 0; i<buttArr.size(); i++){
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                ind = i;
                                goto finish;
                            }
                        }
                        break;
                    case sf::Event::Resized:
                        app.drawMenu(buttArr, textArr);
                        break;
                    case sf::Event::Closed:
                        app.window.close();
                        goto finish;
                    case sf::Event::MouseMoved:
                        translated_pos = app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                buttArr[i].setFillColor(sf::Color(100,100,100,192));
                                lastButtonIndex = i;
                                isInButton = true;
                                app.drawMenu(buttArr, textArr);
                                goto skipisInButton;
                            }
                        }
                        isInButton = false;
                        skipisInButton:
                        if (!isInButton && lastButtonIndex!=-1) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
                            lastButtonIndex = -1;
                            app.drawMenu(buttArr, textArr);
                        }
                    default:
                        break;
                }
            }
        }
    }
    finish:
    buttArr.clear();
    switch (ind){
        case 0:
            switch (playwithhuman()){
                case -1: return -1;
                default: return 0;
            }
        case 1:
            switch (playwithbot()){
                case -1: return -1;
                default: return 0;
            }
        case 2:
            return -1;
        default:
            break;
    }
    return 0;
}

int Menu::playwithbot() {
    app.window.clear();
    app.window.display();
    app.window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    sf::RectangleShape exitButton = app.createButton({300,100});
    exitButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});

    sf::Text exitText = app.createText(font, "BACK", 40, exitButton);

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
        switch (res){
            case -1: return -1; //restart the menu;
            default: std::exit(0);
        }
    } else {
        Game game(hb, p1, bot, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startLoadedGame(currStone);
        switch (res){
            case -1: return -1; //restart the menu;
            default: std::exit(0);
        }
    }
}

int Menu::playwithhuman() {
    app.window.clear();
    app.window.display();
    app.window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    sf::RectangleShape exitButton = app.createButton({300,100});
    exitButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});
    sf::Text exitText = app.createText(font, "BACK", 40, exitButton);

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
        switch (res){
            case -1: return -1; //restart the menu;
            default: std::exit(0);
        }
    } else {
        Game game(hb, p1, p2, app);
        game.setControlButtons(tempButton);
        game.setControlButtonsText(tempText);
        int res = game.startLoadedGame(currStone);
        switch (res){
            case -1: return -1; //restart the menu;
            default: std::exit(0);
        }
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