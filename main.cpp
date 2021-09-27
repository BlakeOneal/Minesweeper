#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <vector>
#include "Tile.h"
#include "Board.h"
#include "Random.h"
#include "TextureManager.h"
using namespace std;


vector<int> readConfig(string fileName) {
    ifstream fileToRead;
    fileToRead = ifstream(fileName);
    vector<int> measurements;
    if (fileToRead.is_open()) {
       string tempInt;
       int width;
       int height;
       int numMines;
       int tempWidth;
       int tempHeight;
       int tileCount;

       getline(fileToRead, tempInt);
       tempWidth = stoi(tempInt);
       width = stoi(tempInt) * 32;
       measurements.push_back(width);

       getline(fileToRead, tempInt);
       tempHeight = stoi(tempInt);
       height = (stoi(tempInt) * 32) + 88;
       measurements.push_back(height);

       getline(fileToRead, tempInt);
       numMines = stoi(tempInt);
       measurements.push_back(numMines);

       tileCount = (tempHeight * tempWidth);
       measurements.push_back(tileCount);

       measurements.push_back(tempWidth);
       measurements.push_back(tempHeight);
    }
    return measurements;
}

int readTestBoard(string fileName, vector<Tile> &tileVector) {
    ifstream fileToRead;
    fileToRead = ifstream(fileName);
    string tempString;
    string characterString;
    int mineCount = 0;
    while(getline(fileToRead, tempString)) {
        characterString+= tempString;
    }
    for (int i = 0; i < characterString.size(); i++) {
        if (characterString[i] == '1') {
            tileVector[i].hasMine(true);
            mineCount++;
        }
        else if (characterString[i] == '0') {
            tileVector[i].hasMine(false);
        }
        tileVector[i].isHidden(true);
        tileVector[i].changeClickStatus(false);
        tileVector[i].isFlagged(false);
    }
    return mineCount;
}

void setAdjacentTiles(vector<Tile> &gameTiles) {
    for (int i = 0; i < gameTiles.size(); i++) {
        gameTiles[i].setAdjacent(gameTiles[i], gameTiles);
        gameTiles[i].isHidden(true);
        gameTiles[i].isFlagged(false);
        gameTiles[i].changeClickStatus(false);
    }
}

void randomizeBoard(vector<Tile> &gameTiles, vector<int> mapMeasure) {
    int numMines;
    for (int i = 0; i < gameTiles.size(); i++) {
        gameTiles[i].isFlagged(false);
        gameTiles[i].changeClickStatus(false);
        gameTiles[i].isHidden(true);
        gameTiles[i].hasMine(false);
    }
    for (numMines = 0; numMines < mapMeasure[2]; numMines = numMines) {
        int randomNumber = Random::Int(0, gameTiles.size() - 1);
        if (!gameTiles[randomNumber].getTileMine()) {
            gameTiles[randomNumber].hasMine(true);
            numMines++;
        }
    }
    setAdjacentTiles(gameTiles);
}


void recursiveReveal(Tile &currentTile) {
    currentTile.changeClickStatus(true);
    currentTile.isHidden(false);
    int numMines = 0;
    int numFlagged = 0;
    Tile* nearbyTiles[8];
    for (int i = 0; i < 8; i++) {
        nearbyTiles[i] = currentTile.getAdjacent(i);
    }
    for (int j = 0; j < 8; j++) {
        if (nearbyTiles[j] != nullptr) {
            if (nearbyTiles[j]->getTileMine()) {
                numMines++;
            }
            if (nearbyTiles[j]->getFlagged()) {
                numFlagged++;
            }
        }
    }
    if (numMines == 0 && !currentTile.getFlagged()) {
        for (int k = 0; k < 8; k++) {
            if (nearbyTiles[k] != nullptr && nearbyTiles[k]->getTileHidden() && !nearbyTiles[k]->getFlagged()) {
                recursiveReveal(*nearbyTiles[k]);
            }
        }
    }
}

map<int, sf::Sprite> loadCounterSprites(sf::Sprite entireCounter) {
    map<int, sf::Sprite> counterTextures;
    for (int i = 0; i < 11; i++) {
        entireCounter.setTextureRect(sf::IntRect(21 * i, 0, 21, 32));
        counterTextures.emplace(i, entireCounter);
    }
    return counterTextures;
}

vector<sf::Sprite> drawCounter(map<int, sf::Sprite> counterSprites, Board gameBoard, vector<Tile> gameTiles) {
    vector<sf::Sprite> spriteToDraw;
    int numMines = gameBoard.getMineCount();
    int numFlaggedTiles = 0;
    for (int i = 0; i < gameTiles.size(); i++) {
        if (gameTiles[i].getFlagged()) {
            numFlaggedTiles++;
        }
    }
    int numberToDisplay = numMines - numFlaggedTiles;
    if (numberToDisplay < 0) {
        spriteToDraw.push_back(counterSprites[10]);
        numberToDisplay *= -1;
    }

    int tempNum = numberToDisplay / 100;
    auto iterator = counterSprites.find(tempNum);
    if (iterator != counterSprites.end()) {
        spriteToDraw.push_back(counterSprites[iterator->first]);
    }
    tempNum = numberToDisplay / 10;
    tempNum %= 10;
    iterator = counterSprites.find(tempNum);
    if (iterator != counterSprites.end()) {
        spriteToDraw.push_back(counterSprites[iterator->first]);
    }
    tempNum = numberToDisplay % 10;
    iterator = counterSprites.find(tempNum);
    if (iterator != counterSprites.end()) {
        spriteToDraw.push_back(counterSprites[iterator->first]);
    }
    return spriteToDraw;
}

int main()
{
    vector<int> mapMeasurements = readConfig("boards/config.cfg");

    sf::RenderWindow window(sf::VideoMode(mapMeasurements[0], mapMeasurements[1]), "Minesweeper");
    sf::Sprite tileHidden(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite tileRevealed(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
    sf::Sprite loserFace(TextureManager::GetTexture("face_lose"));
    sf::Sprite winnerFace(TextureManager::GetTexture("face_win"));
    sf::Sprite testOne(TextureManager::GetTexture("test_1"));
    sf::Sprite testTwo(TextureManager::GetTexture("test_2"));
    sf::Sprite testThree(TextureManager::GetTexture("test_3"));
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));
    sf::Sprite mine(TextureManager::GetTexture("mine"));
    sf::Sprite counter(TextureManager::GetTexture("digits"));
    sf::Sprite one(TextureManager::GetTexture("number_1"));
    sf::Sprite two(TextureManager::GetTexture("number_2"));
    sf::Sprite three(TextureManager::GetTexture("number_3"));
    sf::Sprite four(TextureManager::GetTexture("number_4"));
    sf::Sprite five(TextureManager::GetTexture("number_5"));
    sf::Sprite six(TextureManager::GetTexture("number_6"));
    sf::Sprite seven(TextureManager::GetTexture("number_7"));
    sf::Sprite eight(TextureManager::GetTexture("number_8"));
    map<int, sf::Sprite> counterTextures = loadCounterSprites(counter);
    vector<Tile> tiles;
    Board currentBoard = Board(mapMeasurements[4], mapMeasurements[5], mapMeasurements[3], mapMeasurements[2], false, true, false, false);
    for (int i = 0; i < mapMeasurements[5]; i++) {
        for (int j = 0; j < mapMeasurements[4]; j++) {
            tileHidden.setPosition(TextureManager::GetTexture("tile_hidden").getSize().x * j,
                                   TextureManager::GetTexture("tile_hidden").getSize().y * i);
            Tile tileToAdd = Tile(true, false, false, tileHidden.getPosition().x, tileHidden.getPosition().y, false);
            tiles.push_back(tileToAdd);
        }
    }
        randomizeBoard(tiles, mapMeasurements);

    while (window.isOpen()) {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        for (int i = 0; i < tiles.size(); i++) {
                            sf::IntRect tileArea(tiles[i].getxPos(), tiles[i].getyPos(), 32, 32);
                            if (tileArea.contains(event.mouseButton.x, event.mouseButton.y) && ((!tiles[i].getFlagged() && currentBoard.getCanClick() && tiles[i].getTileHidden()) || (!tiles[i].getFlagged() && currentBoard.getCanClick() && !tiles[i].getTileHidden() && currentBoard.getDebugStatus()))) {
                                tiles[i].isFlagged(true);
                                flag.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                            }
                            else if ((tileArea.contains(event.mouseButton.x, event.mouseButton.y) && tiles[i].getFlagged() && currentBoard.getCanClick())) {
                                tiles[i].isFlagged(false);
                            }
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left) {
                        for (int i = 0; i < tiles.size(); i++) {
                            sf::IntRect tileArea(tiles[i].getxPos(), tiles[i].getyPos(), 32, 32);
                            if (tileArea.contains(event.mouseButton.x, event.mouseButton.y) && tiles[i].getTileHidden() && !tiles[i].getFlagged() && currentBoard.getCanClick() || tileArea.contains(event.mouseButton.x, event.mouseButton.y) && !tiles[i].getTileHidden() && !tiles[i].getFlagged() && currentBoard.getCanClick() && currentBoard.getDebugStatus()) {
                                tiles[i].isHidden(false);
                                tiles[i].changeClickStatus(true);
                                if (tiles[i].getTileMine()) {
                                    currentBoard.changeLost(true);
                                    currentBoard.changeCanClick(false);
                                }
                                int numAdjacentMines = 0;
                                int numFlagged = 0;
                                for (int j = 0; j < 8; j++) {
                                    Tile* currentTile = tiles[i].getAdjacent(j);
                                    if (currentTile != nullptr) {
                                        Tile adjacentTile = *currentTile;
                                        if (adjacentTile.getTileMine()) {
                                            numAdjacentMines++;
                                        }
                                    }
                                }
                                if (numAdjacentMines == 0 && !tiles[i].getTileMine() && !tiles[i].getTileHidden() && numFlagged == 0) {
                                    recursiveReveal(tiles[i]);
                                }
                                }
                            }
                        sf::IntRect debugButton(mapMeasurements[0] - 256, mapMeasurements[1] - 88, 64, 64);
                        if (debugButton.contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (!currentBoard.getDebugStatus() && currentBoard.getCanClick()) {
                                for (int i = 0; i < tiles.size(); i++) {
                                    if (tiles[i].getTileMine()) {
                                        tiles[i].isHidden(false);
                                    }
                                    currentBoard.changeStatus(true);
                            }
                            }
                            else if (currentBoard.getDebugStatus() && currentBoard.getCanClick()) {
                                for (int i = 0; i < tiles.size(); i++) {
                                    if (!tiles[i].getClickedStatus() && tiles[i].getTileMine()) {
                                        tiles[i].isHidden(true);
                                    }
                                    currentBoard.changeStatus(false);
                                }
                            }
                        }
                        sf::IntRect test1Button(mapMeasurements[0] - 192, mapMeasurements[1] - 88, 64, 64);
                        if(test1Button.contains(event.mouseButton.x, event.mouseButton.y)) {
                            int newMineCount = readTestBoard("boards/testboard1.brd", tiles);
                            currentBoard.changeMineCount(newMineCount);
                            currentBoard.changeLost(false);
                            currentBoard.changeGameWon(false);
                            currentBoard.changeCanClick(true);
                            currentBoard.changeStatus(false);
                            setAdjacentTiles(tiles);
                        }
                        sf::IntRect test2Button(mapMeasurements[0] - 128, mapMeasurements[1] - 88, 64, 64);
                        if(test2Button.contains(event.mouseButton.x, event.mouseButton.y)) {
                            int newMineCount = readTestBoard("boards/testboard2.brd", tiles);
                            currentBoard.changeMineCount(newMineCount);
                            currentBoard.changeLost(false);
                            currentBoard.changeGameWon(false);
                            currentBoard.changeCanClick(true);
                            currentBoard.changeStatus(false);
                            setAdjacentTiles(tiles);
                        }
                        sf::IntRect test3Button(mapMeasurements[0] - 64, mapMeasurements[1] - 88, 64, 64);
                        if(test3Button.contains(event.mouseButton.x, event.mouseButton.y)) {
                            int newMineCount = readTestBoard("boards/testboard3.brd", tiles);
                            currentBoard.changeMineCount(newMineCount);
                            currentBoard.changeLost(false);
                            currentBoard.changeGameWon(false);
                            currentBoard.changeCanClick(true);
                            currentBoard.changeStatus(false);
                            setAdjacentTiles(tiles);
                        }
                        sf::IntRect faceButton((mapMeasurements[0] / 2) - 32, mapMeasurements[1] - 88, 64, 64);
                        if(faceButton.contains(event.mouseButton.x, event.mouseButton.y)) {
                            currentBoard.changeGameWon(false);
                            currentBoard.changeMineCount(mapMeasurements[2]);
                            currentBoard.changeLost(false);
                            currentBoard.changeCanClick(true);
                            currentBoard.changeStatus(false);
                            randomizeBoard(tiles, mapMeasurements);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        bool allTilesClicked = true;
        for (int i = 0; i < tiles.size(); i++) {
            if (tiles[i].getTileHidden()) {
                tileHidden.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(tileHidden);
            }
            if (!tiles[i].getTileHidden() && tiles[i].getClickedStatus())
            {
                tileRevealed.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(tileRevealed);
                if (tiles[i].getTileMine()) {
                    mine.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                    window.draw(mine);
                }
            }
            if (!tiles[i].getTileHidden() && !tiles[i].getClickedStatus()) {
                mine.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                tileHidden.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(tileHidden);
                if (tiles[i].getFlagged()) {
                    flag.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                    window.draw(flag);
                }
                window.draw(mine);
            }
            if (tiles[i].getFlagged() && tiles[i].getTileHidden()) {
                flag.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(flag);
            }

            if (!tiles[i].getTileMine() && !tiles[i].getClickedStatus()) {
                allTilesClicked = false;
            }
            int numAdjacentMines = 0;
            int numFlagged = 0;
            for (int j = 0; j < 8; j++) {
                Tile* currentTile = tiles[i].getAdjacent(j);
                if (currentTile != nullptr) {
                    Tile adjacentTile = *currentTile;
                    if (adjacentTile.getTileMine()) {
                        numAdjacentMines++;
                    }
                }
            }
            if (numAdjacentMines == 1 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                one.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(one);
            }
            else if (numAdjacentMines == 2 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                two.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(two);
            }
            else if (numAdjacentMines == 3 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                three.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(three);
            }
            else if (numAdjacentMines == 4 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                four.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(four);
            }
            else if (numAdjacentMines == 5 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                five.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(five);
            }
            else if (numAdjacentMines == 6 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                six.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(six);
            }
            else if (numAdjacentMines == 7 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                seven.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(seven);
            }
            else if (numAdjacentMines == 8 && !tiles[i].getTileMine() && !tiles[i].getTileHidden()) {
                eight.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                window.draw(eight);
            }
        }
        if (allTilesClicked) {
            currentBoard.changeGameWon(true);
        }
        if (currentBoard.getGameLost()) {
            for (int i = 0; i < tiles.size(); i++) {

                if (tiles[i].getTileMine()) {
                    tiles[i].isHidden(false);
                    tileRevealed.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                    window.draw(tileRevealed);
                    if (tiles[i].getFlagged()) {
                        flag.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                        window.draw(flag);
                    }
                    mine.setPosition(tiles[i].getxPos(), tiles[i].getyPos());
                    window.draw(mine);
                }
            }
            loserFace.setPosition((mapMeasurements[0] / 2) - 32, mapMeasurements[1] - 88);
            window.draw(loserFace);
        }
        else if (currentBoard.getGameWon()) {
            int numFlagged = 0;
            bool allClicked = true;
            int numMines = currentBoard.getMineCount();
            for (int i = 0; i < tiles.size(); i++) {
                if (tiles[i].getFlagged()) {
                    numFlagged++;
                }
                if (!tiles[i].getTileMine() && !tiles[i].getClickedStatus()) {
                    allClicked = false;
                }
            }
            if (numFlagged == numMines || allClicked) {
                if (allClicked) {
                    for (int i = 0; i < tiles.size(); i++) {
                        if (tiles[i].getTileMine()) {
                            tiles[i].isFlagged(true);
                        }
                    }
                }
                currentBoard.changeCanClick(false);
                winnerFace.setPosition((mapMeasurements[0] / 2) - 32, mapMeasurements[1] - 88);
                window.draw(winnerFace);
            }
            else {
                currentBoard.changeGameWon(false);
                happyFace.setPosition((mapMeasurements[0] / 2) - 32, mapMeasurements[1] - 88);
                window.draw(happyFace);
            }
        }
        else {
            happyFace.setPosition((mapMeasurements[0] / 2) - 32, mapMeasurements[1] - 88);
            window.draw(happyFace);
        }
        testThree.setPosition(mapMeasurements[0] - 64, mapMeasurements[1] - 88);
        window.draw(testThree);
        testTwo.setPosition(mapMeasurements[0] - 128, mapMeasurements[1] - 88);
        window.draw(testTwo);
        testOne.setPosition(mapMeasurements[0] - 192, mapMeasurements[1] - 88);
        window.draw(testOne);
        debug.setPosition(mapMeasurements[0] - 256, mapMeasurements[1] - 88);
        window.draw(debug);
        vector<sf::Sprite> currentCounterSprites = drawCounter(counterTextures, currentBoard, tiles);
        if (currentCounterSprites.size() == 3) {
            for (int i = 0; i < currentCounterSprites.size(); i++) {
                currentCounterSprites[i].setPosition((21 * i) + 21, mapMeasurements[1] - 88);
                window.draw(currentCounterSprites[i]);
            }
        }
        else {
            for (int i = 0; i < currentCounterSprites.size(); i++) {
                currentCounterSprites[i].setPosition((21 * i), mapMeasurements[1] - 88);
                window.draw(currentCounterSprites[i]);
            }
        }
        window.display();
    }
        return 0;
    }