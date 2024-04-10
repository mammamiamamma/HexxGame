//
// Created by Ilya on 20.02.2024.
//

#include "Menu.h"
#include "Application.h"
#include "Game.h"
#include <iostream>
#include <fstream>

string selectedFile = "nofile"; //can change that

namespace fs = std::filesystem;

Menu::Menu(Application& app)
        : app(app) {
}

void Menu::initializeMainMenuButtons(vector<Button>& buttArr) const {
    Button newGameButton = app.generator.getButton("NEW GAME", 50, sf::Vector2f({500, 150}));
    Button loadGameButton = app.generator.getButton("LOAD GAME", 50, sf::Vector2f({500, 150}));
    Button settingsButton = app.generator.getButton("SETTINGS", 50, sf::Vector2f({500, 150}));
    Button exitButton = app.generator.getButton("EXIT", 50, sf::Vector2f({500, 150}));

    newGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2.f - newGameButton.getShape().getSize().x / 2.f), static_cast<float>(50)});
    loadGameButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - loadGameButton.getShape().getSize().x / 2), static_cast<float>(50 + app.WINDOW_SIZE_Y / 4)});
    settingsButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - exitButton.getShape().getSize().x/2), static_cast<float>(50+2*app.WINDOW_SIZE_Y / 4)});
    exitButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - exitButton.getShape().getSize().x/2), static_cast<float>(50+3*app.WINDOW_SIZE_Y / 4)});

    buttArr.emplace_back(newGameButton);
    buttArr.emplace_back(loadGameButton);
    buttArr.emplace_back(settingsButton);
    buttArr.emplace_back(exitButton);
}

void Menu::initializeNewGameButtons(vector<Button>& buttArr) const {
    Button pvpButton = app.generator.getButton("PL vs PL", 50, {500, 150});
    Button pvbButton = app.generator.getButton("PL vs BOT", 50, {500, 150});
    Button backButton = app.generator.getButton("BACK", 50, {500, 150});

    pvpButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvpButton.getShape().getSize().x / 2), static_cast<float>(100)});
    pvbButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - pvbButton.getShape().getSize().x / 2), static_cast<float>(100 + app.WINDOW_SIZE_Y / 3)});
    backButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - backButton.getShape().getSize().x / 2), static_cast<float>(100 + 2 * app.WINDOW_SIZE_Y / 3)});

    buttArr.emplace_back(pvpButton);
    buttArr.emplace_back(pvbButton);
    buttArr.emplace_back(backButton);
}

void Menu::initializeSettingsButtons(vector<Button>& buttArr, bool isFullscreen) const {
    Button screenModeText = app.generator.getLinelessButton("SCREEN MODE", 40, {500, 150});
    Button screenModeButton;
    if (isFullscreen){
        screenModeButton = app.generator.getButton("FULLSCREEN", 40, {500, 150});
    } else {
        screenModeButton = app.generator.getButton("WINDOWED", 40, {500, 150});
    }
    Button settingsText = app.generator.getLinelessButton("SETTINGS", 50, {500, 150});
    Button backButton = app.generator.getButton("BACK", 50, {500, 150});

    screenModeButton.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 + screenModeText.getShape().getSize().x/2 + 100 - screenModeButton.getShape().getSize().x / 2), static_cast<float>(app.WINDOW_SIZE_Y/2-screenModeButton.getShape().getSize().y/2)});
    screenModeText.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - screenModeText.getShape().getSize().x/2 - 100 - screenModeButton.getShape().getSize().x / 2), static_cast<float>(app.WINDOW_SIZE_Y/2-screenModeButton.getShape().getSize().y/2)});
    settingsText.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - settingsText.getShape().getSize().x / 2), static_cast<float>(50)});
    backButton.setPosition({50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});

    buttArr.emplace_back(settingsText);
    buttArr.emplace_back(screenModeText);
    buttArr.emplace_back(screenModeButton);
    buttArr.emplace_back(backButton);
}

int Menu::registerMouseClick(vector<Button> &buttArr, sf::Vector2f mouse_pos){
    for (int i = 0; i < buttArr.size(); i++) {
        if (buttArr[i].getShape().getGlobalBounds().contains(mouse_pos)) {
            return i;
        }
    }
    return -1;
}

int Menu::registerMouseMove(vector<Button> &buttArr, sf::Vector2f mouse_pos, int ind){
    bool isMouseOverButton = false; // Indicates if the mouse is over any button
    for (int i = 0; i < buttArr.size(); i++) {
        if (buttArr[i].getShape().getGlobalBounds().contains(mouse_pos) && buttArr[i].isActiveButton) {
            buttArr[i].getShape().setFillColor(sf::Color(100,100,100,192));
            ind = i;
            isMouseOverButton = true;
        }
    }
    if (!isMouseOverButton && ind!=-1) {
        buttArr[ind].getShape().setFillColor(sf::Color(0,0,0,192));
        ind = -1;
    }
    return ind;
}

int Menu::launchMenu() {
    vector<Button> buttons;
    initializeMainMenuButtons(buttons);
    app.drawMenu(buttons);
    int ind = -1;
    int lastButtonIndex = -1;
    bool shouldExit = false; //so if shouldExit is false then the loop below should execute (until it is true) and so the check after loop should make sense
    sf::Event event{};
    while (app.window.isOpen() && !shouldExit) {
        while (app.window.pollEvent(event) && !shouldExit) {
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {
                    ind = registerMouseClick(buttons, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)));
                    if (ind!=-1){
                        shouldExit = true;
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    lastButtonIndex = registerMouseMove(buttons, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)), lastButtonIndex);
                    app.drawMenu(buttons);
                }
                case sf::Event::Resized: {
                    app.drawMenu(buttons);
                    break;
                }
                case sf::Event::Closed: return -1;
                default: break;
            }
        }
    }
    if (!app.window.isOpen()) return -1; // checks if the window was closed, maybe change later or maybe its ok
    if (shouldExit){
        buttons[lastButtonIndex].getShape().setFillColor(sf::Color(0,0,0,192));
    }
    return ind;
}

int Menu::launchSettingsMenu(){
    vector<Button> buttArr;
    restart:
    initializeSettingsButtons(buttArr, app.isFullscreen);
    app.drawMenu(buttArr);
    int ind = -1;
    int lastButtonIndex = -1;
    bool shouldExit = false;
    sf::Event event{};
    while (app.window.isOpen() && !shouldExit) {
        while (app.window.pollEvent(event) && !shouldExit) {
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {
                    ind = registerMouseClick(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)));
                    if (ind!=-1 && ind!=0 && ind!=1){ //dont know how foolproof it is but it works for now, need to find another way to ignore these "buttons"
                        shouldExit = true;
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    lastButtonIndex = registerMouseMove(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)), lastButtonIndex);
                    app.drawMenu(buttArr);
                }
                case sf::Event::Resized: {
                    app.drawMenu(buttArr);
                    break;
                }
                case sf::Event::Closed: return -1;
                default: break;
            }
        }
    }
    if (shouldExit) {
        buttArr[lastButtonIndex].getShape().setFillColor(sf::Color(0, 0, 0, 192));
    }
    if (ind == 2){
        app.window.clear(); //maybe rethink this whole segment??
        app.changeState();
        buttArr.clear(); //maybe not??
        goto restart;
    } else {
        return -1;
    }
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

vector<sf::Text> Menu::getTextForButton(const string& filename) const{
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
        inputFile >> gamemode;
        for (int i = 0; i<10; i++){
            std::getline(inputFile, line);
        }
        inputFile >> currentStone;
        inputFile >> freeSpaces;
        inputFile >> score1;
        inputFile >> score2;
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
    vector<Button> buttArr;

    Button saveWindow = app.generator.getButton({740,870});
    saveWindow.setPosition({app.WINDOW_SIZE_X/2-saveWindow.getShape().getSize().x/2, 130});
    saveWindow.setOutlineColor(sf::Color::Cyan);

    buttArr.emplace_back(saveWindow);
    for (int i = 0; i<filenames.size(); i++){
        Button button = app.generator.getButton({700, 150});
        button.setPosition({static_cast<float>(app.WINDOW_SIZE_X / 2 - button.getShape().getSize().x / 2), static_cast<float>(150+ 170*(i%5))});

        vector<sf::Text> buttonText = getTextForButton(filenames[i]);
        buttonText[0].setPosition({button.getShape().getPosition().x+10, button.getShape().getPosition().y+10});
        buttonText[1].setPosition({button.getShape().getPosition().x+10, button.getShape().getPosition().y+button.getShape().getSize().y/2-20});
        buttonText[2].setPosition({button.getShape().getPosition().x+200, button.getShape().getPosition().y+button.getShape().getSize().y/2-20});
        buttonText[3].setPosition({button.getShape().getPosition().x+10, button.getShape().getPosition().y+button.getShape().getSize().y/2+20});
        buttonText[5].setPosition({button.getShape().getPosition().x+255, button.getShape().getPosition().y+button.getShape().getSize().y/2+20});
        buttonText[4].setPosition({button.getShape().getPosition().x+400, button.getShape().getPosition().y+button.getShape().getSize().y/2-20});

        button.addText(buttonText);
        buttArr.emplace_back(button);
    }

    controlButtons.emplace_back(app.generator.getButton("BACK", 40,{300,100},{50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)}));

    if (filenames.size()>5){
        isButtonNeeded = true;
        Button prevButton = app.generator.getButton("PREV PAGE", 30,{300,100},{static_cast<float>(app.WINDOW_SIZE_X/2-700), app.WINDOW_SIZE_Y/2-prevButton.getShape().getSize().y/2});
        Button nextButton = app.generator.getButton("NEXT PAGE", 30,{300,100},{static_cast<float>(app.WINDOW_SIZE_X/2+400), app.WINDOW_SIZE_Y/2-prevButton.getShape().getSize().y/2});

        controlButtons.emplace_back(prevButton);
        controlButtons.emplace_back(nextButton);
    }

    int pageNum = 0;
    sf::Event event{};
    int totalPages = (int)filenames.size()/5;
    int lastButtonIndex = -1;
    bool isInControlButton = false;
    bool shouldExit = false;
    string totalPagesStr;
    {
        stringstream ss;
        ss << totalPages+1;
        ss >> totalPagesStr;
    }
    app.drawMenu(buttArr, controlButtons, pageNum, totalPagesStr, isFirstPage, isLastPage, isButtonNeeded);
    while (app.window.isOpen() && !shouldExit){
        while (app.window.pollEvent(event) && !shouldExit){
            switch (event.type){
                case sf::Event::Closed:{
                    std::exit(1);
                }
                case sf::Event::MouseButtonPressed: {
                    int n = -1;
                    for (int i = 0; i<3; i++){
                        if (controlButtons[i].getShape().getGlobalBounds().contains(app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
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
                        if (buttArr[i].getShape().getGlobalBounds().contains(app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            selectedFile = filenames[i-1];
                            shouldExit = true;
                            break;
                        }
                    }
                    app.drawMenu(buttArr, controlButtons, pageNum, totalPagesStr, isFirstPage, isLastPage, isButtonNeeded);
                    break;
                }
                case sf::Event::MouseMoved: {
                    if (lastButtonIndex != -1) {
                        if (!isInControlButton && !buttArr[lastButtonIndex].getShape().getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            buttArr[lastButtonIndex].getShape().setFillColor(sf::Color(0, 0, 0, 192));
                            lastButtonIndex = -1;
                        } else if (isInControlButton && !controlButtons[lastButtonIndex].getShape().getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            controlButtons[lastButtonIndex].getShape().setFillColor(sf::Color(0, 0, 0, 192));
                            isInControlButton = false;
                            lastButtonIndex = -1;
                        }
                    }
                    for (int i = 1 + pageNum * 5, j = i; i < j + 5 && i < buttArr.size(); i++) {
                        if (buttArr[i].getShape().getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            buttArr[i].getShape().setFillColor(sf::Color(100, 100, 100, 192));
                            lastButtonIndex = i;
                            goto skipisInButton;
                        }
                    }
                    for (int i = 0; i < controlButtons.size(); i++) {
                        if (controlButtons[i].getShape().getGlobalBounds().contains(
                                app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)))) {
                            controlButtons[i].getShape().setFillColor(sf::Color(100, 100, 100, 192));
                            lastButtonIndex = i;
                            app.drawMenu(buttArr, controlButtons, pageNum, totalPagesStr, isFirstPage, isLastPage, isButtonNeeded);
                            isInControlButton = true;
                        }
                    }
                    skipisInButton:
                    app.drawMenu(buttArr, controlButtons, pageNum, totalPagesStr, isFirstPage, isLastPage, isButtonNeeded);
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
    vector<Button> buttArr;
    initializeNewGameButtons(buttArr);
    app.drawMenu(buttArr);
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
                    app.drawMenu(buttArr);
                    break;
                }
                case sf::Event::Closed:{
                    shouldExit = true;
                    app.window.close();
                    break;
                }
                case sf::Event::MouseMoved: {
                    lastButtonIndex = registerMouseMove(buttArr, app.window.mapPixelToCoords(sf::Mouse::getPosition(app.window)), lastButtonIndex);
                    app.drawMenu(buttArr);
                }
                default: break;
            }
        }
    }
    buttArr.clear();
    return ind;
}

void Menu::initializeGame(const bool isLoaded, const bool isPVP){ //isPVP only matters if the game is launched as new, can we change that?
    app.window.clear();
    app.window.display();
    app.window.clear(); //fixed buttons appearing behind the board for me, maybe leave it like this?
    Button exitButton = app.generator.getButton("BACK", 40,{300,100},{50, static_cast<float>(app.WINDOW_SIZE_Y-80-100)});
    vector<Button> tempButton;
    tempButton.emplace_back(exitButton);

    HexBoard hb = HexBoard();
    unique_ptr<Player> p1 = make_unique<Player>(Stone::BLUE);
    unique_ptr<Player> p2;
    Stone currStone = Stone::BLUE; //default value overriden if game is loaded

    if (!isLoaded){
        if (isPVP) p2 = make_unique<Player>(Stone::RED);
        else p2 = make_unique<Bot>(Stone::RED);
        Game game(hb, *p1, *p2, app);
        game.setControlButtons(tempButton);
        game.startGame();
    } else {
        loadGame(selectedFile, p1, p2, hb, currStone);
        Game game(hb, *p1, *p2, app);
        game.setControlButtons(tempButton);
        game.startLoadedGame(currStone);
    }
}

void Menu::loadGame(const string& filename, const unique_ptr<Player>& p1, unique_ptr<Player>& p2, HexBoard& hb, Stone& currStone) { //make it accept players? accept filename?
    std::ifstream inputFile("saves/"+filename);
    if (inputFile.is_open()) {
        string temp;
        inputFile >> temp;
        if (temp == "Bot") {
            p2 = make_unique<Bot>(Stone::RED);
        }
        std::string line;
        std::getline(inputFile, line); // necessary flush of the buffer or something
        for (int row = 0; row < HexBoard::BOARD_SIZE; ++row) {
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
        p1->setScore(score);
        inputFile >> score;
        p2->setScore(score);
        inputFile.close();
    } else {
        std::cerr << "Error: Cannot open the file" << std::endl;
    }
}

void Menu::ClearControlButtons() {
    controlButtons.clear();
}

void Menu::ResetMenuState() {
    isFirstPage = true;
    isLastPage = false;
    isButtonNeeded = false;
}