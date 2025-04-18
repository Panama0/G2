#pragma once

#include <cstdint>
#include <string>

struct TileEffect
{
    enum Effects
    {
        none,
        water,
        obstructed,
        path,
        spawner
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

    TileEffect(TileEffect::Effects _effect) : effect{_effect}
    {
        id = m_idCounter++;
    }

    std::string textureName;
    Effects effect;
    uint32_t id{};

private:
    static inline uint32_t m_idCounter{};
};