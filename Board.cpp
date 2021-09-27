#include "Board.h"

Board::Board(int _numColumns, int _numRows, int _tileCount, int _mineCount, bool _isDebugged, bool _canClick, bool _gameLost, bool _gameWon) {
    numColumns = _numColumns;
    numRows = _numRows;
    tileCount = _tileCount;
    mineCount = _mineCount;
    isDebugged = _isDebugged;
    canClick = _canClick;
    gameLost = _gameLost;
    gameWon = _gameWon;
}

int Board::getMineCount() {
    return mineCount;
}

bool Board::getDebugStatus() {
    return isDebugged;
}

bool Board::getGameLost() {
    return gameLost;
}

bool Board::getCanClick() {
    return canClick;
}

bool Board::getGameWon() {
    return gameWon;
}

void Board::changeStatus(bool debuggedToggle) {
    isDebugged = debuggedToggle;
}

void Board::changeMineCount(int newCount) {
    mineCount = newCount;
}

void Board::changeLost(bool gameLoser) {
    gameLost = gameLoser;
}

void Board::changeGameWon(bool gameWinner) {
    gameWon = gameWinner;
}

void Board::changeCanClick(bool cannotClick) {
    canClick = cannotClick;
}
