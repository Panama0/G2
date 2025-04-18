#pragma once

#include "SFML/Graphics.hpp"
#include "engine/EntityManager.hpp"
#include "scene base/GameMap.hpp"

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

    Modes currentMode{Modes::none};

    bool gridVisible{true};
    bool brushesVisible{true};

    std::string tileTexture;
    TileEffect::Effects brushType{TileEffect::none};

    //* should be a reference or ptr?
    GameMap::MapTile selectedTile;

    sf::Angle angle;
    GameMap map;

    sf::Vector2u gridSize;

    std::vector<std::string> textureList;

    std::string fileName{"Unnamed Level"};
    std::filesystem::path filePath{"../../res/saves/"};
};