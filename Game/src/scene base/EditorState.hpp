#pragma once

#include "GameMap.hpp"
#include "SFML/System/Angle.hpp"
#include "Vec2.hpp"

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
        std::optional<Vec2i> lastPlaced;

        void reset()
        {
            state = none;
            lastPlaced = std::nullopt;
        }
    };
    Mouse mouseState;

    bool brushesVisible{true};

    std::string tileTexture;
    TileEffect::Effects brushType{TileEffect::none};

    std::shared_ptr<GameMap::MapTile> selectedTile;
    Vec2f selectedTilePos;

    sf::Angle angle;
    GameMap map;

    std::vector<std::string> textureList;

    std::string saveName{"UnnamedLevel"};
    std::filesystem::path savePath{"../../../Game/res/saves"};
};
