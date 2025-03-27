#pragma once

#include "SFML/Graphics.hpp"
#include "scene base/GameMap.hpp"
#include "engine/EntityManager.hpp"

#include <string>
#include <vector>

struct EditorState
{    
    // some modes for the editor, format is leftclickRightclick
    enum class Modes
    {
        none,
        tilePlaceTileRemove,
        brushPlaceBrushRemove,
        selectNone,
    };
    
    Modes currentMode {Modes::none};
    
    bool gridVisible {true};
    
    std::string tileTexture;
    GameMap::Brush::BrushTypes brushType;
    
    std::vector<GameMap::MapTile> selectedTiles;
    std::vector<GameMap::Brush> selectedBrushes;
    
    sf::Angle angle;
    GameMap map;
    
    sf::Vector2u gridSize;
    
    std::vector<std::string> textureList;

    std::string fileName {"Unnamed Level"};
    std::filesystem::path filePath {"../../res/saves/"};
};