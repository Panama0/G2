#pragma once

#include "SFML/Graphics.hpp"
#include "scene base/GameMap.hpp"

#include <string>
#include <vector>

struct EditorState
{
    bool gridVisible {true};
    bool placing {true};
    std::string selectedTile;
    sf::Angle angle;
    GameMap map;
    
    sf::Vector2u gridSize;
    
    std::vector<std::string> textureList;

    std::string fileName {"Unnamed Level"};
    std::filesystem::path filePath {"../../res/saves/"};
};