using namespace std;
#include <iostream>
#pragma once
#include "Tile.h"

class Board {
private:
    unsigned int numColumns = 0;
    unsigned int numRows = 0;
    unsigned int mineCount;
    unsigned int tileCount;
    bool gameWon = false;
    bool isDebugged = false;
    bool canClick = true;
    bool gameLost = false;
public:
    Board(int numColumns, int numRows, int tileCount, int mineCount, bool isDebugged, bool canClick, bool gameLost, bool gameWon);
    bool getDebugStatus();
    bool getGameLost();
    bool getCanClick();
    bool getGameWon();
    int getMineCount();
    void changeGameWon(bool gameWinner);
    void changeStatus(bool debuggedToggle);
    void changeLost(bool gameLoser);
    void changeCanClick(bool cannotClick);
    void changeMineCount(int newCount);
};

