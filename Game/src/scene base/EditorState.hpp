#pragma once

#include "GameMap.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

struct EditorState
{
    // some modes for the editor, format is leftclickRightclick
    enum class Mode
    {
        none,
        tilePlaceTileRemove,
        brushPlaceBrushRemove,
        selectNone,
    };

    Mode currentMode{Mode::none};

    struct Mouse
    {
        enum State
        {
            none,
            leftDown,
            rightDown
        };

        State state{none};
        std::optional<sf::Vector2u> lastPlaced;

        void reset()
        {
            state = none;
            lastPlaced = std::nullopt;
        }
    };
    Mouse mouseState;

    bool brushesVisible{true};

    std::string tileTexture;
    TileEffect brushType{TileEffect::none};

    std::shared_ptr<GameMap::MapTile> selectedTile;
    sf::Vector2i selectedTilePos;

    sf::Angle angle;
    GameMap map;

    std::vector<std::string> textureList;

    std::string saveName{"UnnamedLevel"};
    std::filesystem::path savePath{"../../../Game/res/saves"};
};
