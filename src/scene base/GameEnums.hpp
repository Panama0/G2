#pragma once

#include <string_view>

namespace enums
{
    enum class Brushes
    {
        water,
        obstructed,
        path,
        spawner,
        
        
        
        count,
    };
    
    constexpr std::string_view toString(Brushes brush)
    {
        switch(brush)
        {
            case Brushes::water:
                return "Water";
            case Brushes::obstructed:
                return "Obstructed";
            case Brushes::path:
                return "Path";
            case Brushes::spawner:
                return "Spawner";
            default:
                return "Invalid\n";
        }
    }
}
