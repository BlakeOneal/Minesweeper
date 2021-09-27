#include "Tile.h"

Tile::Tile(bool _tileHidden, bool _tileFlagged, bool _tileMine, int _xPos, int _yPos, bool _hasBeenClicked) {
    tileHidden = _tileHidden;
    tileFlagged = _tileFlagged;
    tileMine = _tileMine;
    xPos = _xPos;
    yPos = _yPos;
    hasBeenClicked = _hasBeenClicked;
}

Tile::Tile() {

}

int Tile::getxPos() {
    return xPos;
}

int Tile::getyPos() {
    return yPos;
}

bool Tile::getFlagged() {
    return tileFlagged;
}

bool Tile::getTileHidden() {
    return tileHidden;
}

bool Tile::getTileMine() {
    return tileMine;
}

bool Tile::getClickedStatus() {
    return hasBeenClicked;
}

void Tile::isFlagged(bool _flagged) {
    tileFlagged = _flagged;
}

void Tile::isHidden(bool hidden) {
    tileHidden = hidden;
}

void Tile::hasMine(bool mine) {
    tileMine = mine;
}

void Tile::changeClickStatus(bool clicked) {
    hasBeenClicked = clicked;
}

void Tile::setAdjacent(Tile currentTile, vector<Tile> &boardTiles) {
    Tile* nearby[8];
    for (int iterator = 0; iterator < 8; iterator++) {
        nearby[iterator] = nullptr;
    }
    int currentNumPointers = 0;
    for (int i = 0; i < boardTiles.size(); i++) {
            if (boardTiles[i].getxPos() == currentTile.getxPos() + (32) && boardTiles[i].getyPos() == currentTile.getyPos()) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getxPos() == currentTile.getxPos() - (32) && boardTiles[i].getyPos() == currentTile.getyPos()) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getyPos() == currentTile.getyPos() + (32) && boardTiles[i].getxPos() == currentTile.getxPos()) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getyPos() == currentTile.getyPos() - (32) && boardTiles[i].getxPos() == currentTile.getxPos()) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getxPos() == currentTile.getxPos() + (32) && boardTiles[i].getyPos() == currentTile.getyPos() + (32)) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getxPos() == currentTile.getxPos() - (32) && boardTiles[i].getyPos() == currentTile.getyPos() - (32)) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getxPos() == currentTile.getxPos() + (32) && boardTiles[i].getyPos() == currentTile.getyPos() - (32)) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
            else if (boardTiles[i].getxPos() == currentTile.getxPos() - (32) && boardTiles[i].getyPos() == currentTile.getyPos() + (32)) {
                nearby[currentNumPointers] = &boardTiles[i];
                currentNumPointers++;
            }
    }
    for (int i = 0; i < 8; i++) {
        adjacentTiles[i] = nearby[i];
    }
}

Tile* Tile::getAdjacent(int index) {
    return adjacentTiles[index];
}

