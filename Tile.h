using namespace std;
#include <iostream>
#include <vector>
#pragma once
#include <SFML/Graphics.hpp>

class Tile {
private:
    bool tileHidden = true;
    bool tileFlagged = false;
    bool tileMine = false;
    bool hasBeenClicked = false;
    int xPos = 0;
    int yPos = 0;
    Tile* adjacentTiles[8];
public:
    Tile(bool tileHidden, bool tileFlagged, bool tileMine, int xPos, int yPos, bool hasBeenClicked);
    Tile();
    int getxPos();
    int getyPos();
    bool getFlagged();
    bool getTileHidden();
    bool getTileMine();
    void setAdjacent(Tile currentTile, vector<Tile> &gameTiles);
    Tile* getAdjacent(int index);
    bool getClickedStatus();
    void isFlagged(bool flagged);
    void isHidden(bool hidden);
    void hasMine(bool mine);
    void changeClickStatus(bool clicked);
};
