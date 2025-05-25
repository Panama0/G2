#pragma once

#include <string_view>

// Any effect that we may want to apply to a tile
struct TileEffect
{
    enum Effects
    {
        none,
        water,
        obstructed,
        path,
        spawner,

        count // total number of effects
    };

    static constexpr std::string_view toString(Effects brush)
    {
        switch(brush)
        {
        case Effects::none:
            return "None";
        case Effects::water:
            return "Water";
        case Effects::obstructed:
            return "Obstructed";
        case Effects::path:
            return "Path";
        case Effects::spawner:
            return "Spawner";
        default:
            return "Invalid\n";
        }
    }

    static constexpr Effects fromString(std::string_view string)
    {
        if(string == "None")
        {
            return Effects::none;
        }
        else if(string == "Water")
        {
            return Effects::water;
        }
        else if(string == "Obstructed")
        {
            return Effects::obstructed;
        }
        else if(string == "Path")
        {
            return Effects::path;
        }
        else if(string == "Spawner")
        {
            return Effects::spawner;
        }
        else
        {
            return Effects::none;
        }
    }

    TileEffect() = default;
    TileEffect(Effects eff) : effect{eff} {}

    Effects effect;
};
