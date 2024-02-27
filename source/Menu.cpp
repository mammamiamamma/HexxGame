//
// Created by Ilya on 20.02.2024.
//

#include "Menu.h"
#include "Application.h"
#include "Game.h"
#include <iostream>
#include <fstream>

bool isFirstPage = true;
bool isLastPage = false;
bool isButtonNeeded = false;

vector<sf::RectangleShape> controlButtons;
vector<sf::Text> controlButtonsText;
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
Menu::Menu(sf::RenderWindow& window, sf::Font& font, sf::RectangleShape& bg)
        : window(window), font(font), background(bg) {
}

void Menu::runMenu() {
    if (launchMenu() == -1) {
        window.close();
    }
}

int Menu::launchMenu() {
    vector<sf::RectangleShape> buttArr;
    vector<sf::Text> textArr;
    sf::RectangleShape newGameButton;
    sf::RectangleShape loadGameButton;
    sf::RectangleShape exitButton;

    newGameButton.setSize({500, 150});
    loadGameButton.setSize({500, 150});
    exitButton.setSize({500,150});

    newGameButton.setOutlineThickness(5);
    loadGameButton.setOutlineThickness(5);
    exitButton.setOutlineThickness(5);

    newGameButton.setOutlineColor(sf::Color::White);
    loadGameButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineColor(sf::Color::White);

    newGameButton.setFillColor(sf::Color(0,0,0,192));
    loadGameButton.setFillColor(sf::Color(0,0,0,192));
    exitButton.setFillColor(sf::Color(0,0,0,192));

    newGameButton.setPosition({static_cast<float>(window.getSize().x / 2 - newGameButton.getSize().x / 2), static_cast<float>(100)});
    loadGameButton.setPosition({static_cast<float>(window.getSize().x / 2 - loadGameButton.getSize().x / 2), static_cast<float>(100 + window.getSize().y / 3)});
    exitButton.setPosition({static_cast<float>(window.getSize().x / 2 - exitButton.getSize().x/2), static_cast<float>(100+2*window.getSize().y / 3)});

    sf::Text newGameText(font, "NEW GAME", 50);
    sf::Text loadGameText(font, "LOAD GAME", 50);
    sf::Text exitText(font, "EXIT", 50);

    newGameText.setPosition({static_cast<float>(newGameButton.getPosition().x + 50), static_cast<float>(newGameButton.getPosition().y + newGameButton.getSize().y / 4)});
    loadGameText.setPosition({static_cast<float>(loadGameButton.getPosition().x + 30), static_cast<float>(loadGameButton.getPosition().y + loadGameButton.getSize().y / 4)});
    exitText.setPosition({static_cast<float>(exitButton.getPosition().x+165), static_cast<float>(exitButton.getPosition().y+exitButton.getSize().y/4)});

    buttArr.push_back(newGameButton);
    buttArr.push_back(loadGameButton);
    buttArr.push_back(exitButton);

    textArr.push_back(newGameText);
    textArr.push_back(loadGameText);
    textArr.push_back(exitText);

    restart:
    drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool isInButton = false;
    {
        sf::Event event{};
        sf::Vector2f translated_pos;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                ind = i;
                                goto finish;
                            }
                        }
                        break;
                    case sf::Event::Resized:
                        drawMenu(buttArr, textArr);
                        break;
                    case sf::Event::Closed:
                        return -1;
                    case sf::Event::MouseMoved:
                        translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                buttArr[i].setFillColor(sf::Color(100,100,100,192));
                                lastButtonIndex = i;
                                isInButton = true;
                                drawMenu(buttArr, textArr);
                                goto skipisInButton;
                            }
                        }
                        isInButton = false;
                    skipisInButton:
                        if (!isInButton && lastButtonIndex!=-1) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
                            lastButtonIndex = -1;
                            drawMenu(buttArr, textArr);
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
            window.clear();
            switch (launchNewGameMenu()){
                case 1: playwithhuman();
                case 2: playwithbot();
                case -1: {
                    goto restart;
                }
                case 0: return -1;
            }
            break;
        case 1:
            switch (launchLoadGameMenu()){
//                case 0: return -1;
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
//            return -1;
            break;
        case 2:
            return -1;
    }
    buttArr.clear();
    return 0;
}

void Menu::drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr) {
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

void Menu::drawMenu(const std::vector<sf::RectangleShape>& buttArr, const std::vector<sf::Text>& textArr, int pageNum) {
    stringstream ss;
    ss << pageNum+1;
    string pageStr;
    ss >> pageStr;
    window.clear();
    window.draw(background);
    window.draw(buttArr[0]);

    for (int i = 1 + pageNum*5, j = i; i<j+5 && i<buttArr.size(); i++){
        window.draw(buttArr[i]);
    }
    for (int i = pageNum*30, j = i; i<j+30 && i<textArr.size(); i++){
        window.draw(textArr[i]);
    }
    sf::Text savefilestext(font, "SAVE FILES", 30);
    savefilestext.setPosition({static_cast<float>(window.getSize().x/2-255/2),80});
    window.draw(savefilestext);
    sf::Text pageIndexText(font, "PAGE "+pageStr + " OF " + totalPagesStr, 30);
    pageIndexText.setPosition({static_cast<float>(window.getSize().x/2-255/2),static_cast<float>(window.getSize().y-70)});
    window.draw(pageIndexText);

    drawButton(controlButtons[0], controlButtonsText[0]);

    if (isButtonNeeded) {
        if (!isLastPage) drawButton(controlButtons[2], controlButtonsText[2]);
        if (!isFirstPage) drawButton(controlButtons[1], controlButtonsText[1]);
    }
    window.display();
}

void Menu::drawButton(const sf::RectangleShape& b, const sf::Text& text) {
    window.draw(b);
    window.draw(text);
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
    string currStone;
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
        inputFile >> currStone;
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

        sf::Text stoneText(font, currStone, 20);
        stoneText.setOutlineThickness(2);
        if (currStone=="B") {
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

    sf::RectangleShape saveWindow;
    saveWindow.setSize({740,870});
    saveWindow.setPosition({window.getSize().x/2-saveWindow.getSize().x/2, 130});
    saveWindow.setOutlineThickness(5);
    saveWindow.setOutlineColor(sf::Color::Cyan);
    saveWindow.setFillColor(sf::Color(0,0,0,192));
    buttArr.push_back(saveWindow);

    for (int i = 0; i<filenames.size(); i++){
        sf::RectangleShape button;
        button.setSize({700, 150});
        button.setOutlineThickness(5);
        button.setOutlineColor(sf::Color::White);
        button.setFillColor(sf::Color(0,0,0,192));
        button.setPosition({static_cast<float>(window.getSize().x / 2 - button.getSize().x / 2), static_cast<float>(150+ 170*(i%5))});
        buttArr.push_back(button);

        vector<sf::Text> buttonText = getTextForButton(filenames[i]);
        buttonText[0].setPosition({button.getPosition().x+10, button.getPosition().y+10});
        buttonText[1].setPosition({button.getPosition().x+10, button.getPosition().y+button.getSize().y/2-20});
        buttonText[2].setPosition({button.getPosition().x+200, button.getPosition().y+button.getSize().y/2-20});
        buttonText[3].setPosition({button.getPosition().x+10, button.getPosition().y+button.getSize().y/2+20});
        buttonText[5].setPosition({button.getPosition().x+255, button.getPosition().y+button.getSize().y/2+20});
        buttonText[4].setPosition({button.getPosition().x+400, button.getPosition().y+button.getSize().y/2-20});

        textArr.push_back(buttonText[0]);
        textArr.push_back(buttonText[1]);
        textArr.push_back(buttonText[2]);
        textArr.push_back(buttonText[3]);
        textArr.push_back(buttonText[5]);
        textArr.push_back(buttonText[4]);
    }

    sf::RectangleShape backButton;
    backButton.setSize({300,100});
    backButton.setPosition({50, static_cast<float>(window.getSize().y-80-100)});
    backButton.setOutlineThickness(5);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setFillColor(sf::Color(0,0,0,192));

    sf::Text backText(font, "BACK", 40);
    backText.setPosition({static_cast<float>(backButton.getPosition().x+backButton.getSize().x/2-80), static_cast<float>(backButton.getPosition().y+backButton.getSize().y/2-30)});


    controlButtons.push_back(backButton);
    controlButtonsText.push_back(backText);

    if (filenames.size()>5){
        isButtonNeeded = true;
        sf::RectangleShape prevButton;
        sf::RectangleShape nextButton;
        prevButton.setSize({300,100});
        nextButton.setSize({300,100});
        prevButton.setPosition({static_cast<float>(window.getSize().x/2-700), window.getSize().y/2-prevButton.getSize().y/2});
        nextButton.setPosition({static_cast<float>(window.getSize().x/2+400), window.getSize().y/2-prevButton.getSize().y/2});
        prevButton.setOutlineThickness(5);
        nextButton.setOutlineThickness(5);
        prevButton.setOutlineColor(sf::Color::White);
        nextButton.setOutlineColor(sf::Color::White);
        prevButton.setFillColor(sf::Color(0,0,0,192));
        nextButton.setFillColor(sf::Color(0,0,0,192));

        sf::Text prevButtonText(font, "PREV PAGE", 30);
        sf::Text nextButtonText(font, "NEXT PAGE", 30);
        prevButtonText.setPosition({prevButton.getPosition().x+20, prevButton.getPosition().y+30});
        nextButtonText.setPosition({nextButton.getPosition().x+20, nextButton.getPosition().y+30});

        controlButtons.push_back(prevButton);
        controlButtons.push_back(nextButton);

        controlButtonsText.push_back(prevButtonText);
        controlButtonsText.push_back(nextButtonText);
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
    drawMenu(buttArr, textArr, pageNum);
    while (window.isOpen()){
        while (window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:{
                    std::exit(1);
                }
                case sf::Event::MouseButtonPressed:{
                    translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
                            drawMenu(buttArr, textArr, pageNum);
                            goto skip;
                        }
                    }
                    skip:
                    break;
                }
                case sf::Event::MouseMoved:
                    translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
                            drawMenu(buttArr, textArr, pageNum);
                            isInControlButton = true;
                        }
                    }
                    skipisInButton:
                    drawMenu(buttArr, textArr, pageNum);
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
    sf::RectangleShape pvpButton;
    sf::RectangleShape pvbButton;
    sf::RectangleShape backButton;

    pvpButton.setSize({500, 150});
    pvbButton.setSize({500, 150});
    backButton.setSize({500, 150});

    pvpButton.setOutlineThickness(5);
    pvbButton.setOutlineThickness(5);
    backButton.setOutlineThickness(5);

    pvpButton.setOutlineColor(sf::Color::White);
    pvbButton.setOutlineColor(sf::Color::White);
    backButton.setOutlineColor(sf::Color::White);

    pvpButton.setFillColor(sf::Color(0,0,0,192));
    pvbButton.setFillColor(sf::Color(0,0,0,192));
    backButton.setFillColor(sf::Color(0,0,0,192));

    pvpButton.setPosition({static_cast<float>(window.getSize().x / 2 - pvpButton.getSize().x / 2), static_cast<float>(100)});
    pvbButton.setPosition({static_cast<float>(window.getSize().x / 2 - pvbButton.getSize().x / 2), static_cast<float>(100 + window.getSize().y / 3)});
    backButton.setPosition({static_cast<float>(window.getSize().x / 2 - backButton.getSize().x / 2), static_cast<float>(100 + 2 * window.getSize().y / 3)});

    sf::Text pvpText(font, "Pl vs Pl", 50);
    sf::Text pvbText(font, "Pl vs Bot", 50);
    sf::Text backText(font, "BACK", 50);

    pvpText.setPosition({static_cast<float>(pvpButton.getPosition().x + (pvpButton.getSize().x - 330) / 2), static_cast<float>(pvpButton.getPosition().y + pvpButton.getSize().y / 4)});
    pvbText.setPosition({static_cast<float>(pvbButton.getPosition().x + (pvbButton.getSize().x - 380) / 2), static_cast<float>(pvbButton.getPosition().y + pvbButton.getSize().y / 4)});
    backText.setPosition({static_cast<float>(backButton.getPosition().x + (backButton.getSize().x - 200) / 2), static_cast<float>(backButton.getPosition().y + backButton.getSize().y / 4)});

    buttArr.push_back(pvpButton);
    buttArr.push_back(pvbButton);
    buttArr.push_back(backButton);

    textArr.push_back(pvpText);
    textArr.push_back(pvbText);
    textArr.push_back(backText);

    drawMenu(buttArr, textArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool isInButton;
    {
        sf::Event event{};
        sf::Vector2f translated_pos;
        while (window.isOpen()){
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::MouseButtonPressed:
                        translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        for (int i = 0; i<buttArr.size(); i++){
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                ind = i;
                                goto finish;
                            }
                        }
                        break;
                    case sf::Event::Resized:
                        drawMenu(buttArr, textArr);
                        break;
                    case sf::Event::Closed:
                        window.close();
                        goto finish;
                    case sf::Event::MouseMoved:
                        translated_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        for (int i = 0; i < buttArr.size(); i++) {
                            if (buttArr[i].getGlobalBounds().contains(translated_pos)) {
                                buttArr[i].setFillColor(sf::Color(100,100,100,192));
                                lastButtonIndex = i;
                                isInButton = true;
                                drawMenu(buttArr, textArr);
                                goto skipisInButton;
                            }
                        }
                        isInButton = false;
                        skipisInButton:
                        if (!isInButton && lastButtonIndex!=-1) {
                            buttArr[lastButtonIndex].setFillColor(sf::Color(0,0,0,192));
                            lastButtonIndex = -1;
                            drawMenu(buttArr, textArr);
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
            return 1;
        case 1:
            return 2;
        case 2:
            return -1;
        default:
            break;
    }
    return 0;
}

void Menu::playwithbot() {
    window.clear();
    window.display();
    window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    Application app(window, hb, background);
    if (!isLoadedGame){
        hb = HexBoard();
        p1 = Player(Stone::BLUE);
        bot = Bot(Stone::RED);

        Game game(hb, p1, bot, app);
        game.startGame();
    } else {
        Game game(hb, p1, bot, app);
        game.startLoadedGame(currStone);
    }
}

void Menu::playwithhuman() {
    window.clear();
    window.display();
    window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    Application app(window, hb, background);
    if (!isLoadedGame){
        hb = HexBoard();
        p1 = Player(Stone::BLUE);
        p2 = Player(Stone::RED);

        Game game(hb, p1, p2, app);
        game.startGame();
    } else {
        Game game(hb, p1, p2, app);
        game.startLoadedGame(currStone);
    }
}

int Menu::loadGame() {
    if (selectedFile == "nofile") return -1;
    isLoadedGame = true;
    p1 = Player(Stone::BLUE);
    p2 = Player(Stone::RED);

    std::ifstream inputFile("saves/"+selectedFile);
    if (inputFile.is_open()) {
        for (int row = 0; row < HexBoard::BOARD_SIZE; ++row) {
            std::string line;
            if (!std::getline(inputFile, line)) {
                break;  // Reached the end of the file
            }
            for (size_t col = 0; col < HexBoard::BOARD_SIZE; ++col) {
                if (col >= line.length()) {
                    break;  // Reached the end of the line
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
