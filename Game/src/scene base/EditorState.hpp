#pragma once

#include "GameMap.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

#include <string>
#include <vector>
#include <optional>

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
    TileEffect brushType{TileEffect::none};

    std::optional<GameMap::MapTile> selectedTile;
    sf::Vector2i selectedTilePos;

    sf::Angle angle;
    GameMap map;

    sf::Vector2u gridSize;

    std::vector<std::string> textureList;

    std::string saveName{"UnnamedLevel"};
    std::filesystem::path savePath{"../../../Game/res/saves"};
};
