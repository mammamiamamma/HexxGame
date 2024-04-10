//
// Created by Ilya on 20.02.2024.
//

#ifndef MENU_H
#define MENU_H

#include "SFML/Graphics.hpp"
#include "Application.h"

// note for me: return -1 for "go back", return 0 for i dunno exit i guess (yeah its returned when default and what not), return 1 (in case of loadgame its a successful loading game)
class Menu {
public:
    // should probably add vectors for buttons so i dont create and destroy vectors in each method, but will require proper clearing
    Application& app;
    explicit Menu(Application& app);
    int launchMenu();
    int launchNewGameMenu();
    int launchLoadGameMenu();
    int launchSettingsMenu();
    static std::vector<std::string> getFileNames();
    static std::vector<std::string> split(const std::string& s);
    void initializeGame(bool isLoaded, bool isPVP);
    [[nodiscard]] std::vector<sf::Text> getTextForButton(const std::string& filename) const;
    static void loadGame(const string& filename, const unique_ptr<Player>& p1, unique_ptr<Player>& p2, HexBoard& hb, Stone& currStone);
    void initializeMainMenuButtons(vector<Button>& buttArr) const;
    void initializeNewGameButtons(vector<Button>& buttArr) const;
    void initializeSettingsButtons(vector<Button>& buttArr, bool isFullscreen) const;
    static int registerMouseMove(vector<Button>& buttArr, sf::Vector2f mouse_pos, int ind);
    static int registerMouseClick(vector<Button>& buttArr, sf::Vector2f mouse_pos);
    void updatePageButtons(int pageNum, int totalPages);
    void ClearControlButtons();
    void ResetMenuState();
private:
    std::vector<Button> controlButtons;
    bool isFirstPage = true;
    bool isLastPage = false;
    bool isButtonNeeded = false;
};

#endif // MENU_H
