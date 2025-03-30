#pragma once

#include <string>
#include <cstdint>

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
    
    TileEffect(TileEffect::Effects _effect)
        :effect {_effect}
    {
        id = m_idCounter++;
    }
    
    std::string textureName;
    Effects effect;
    uint32_t id {};
private:
    static inline uint32_t m_idCounter {};
};