#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

class TextureManager {
private:
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string textureName);
public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear();

};
