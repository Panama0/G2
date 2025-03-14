#pragma once

#include "scene base/Assets.hpp"

#include "SFML/Graphics.hpp"

#include <filesystem>
#include <vector>

class GameMap
{
public:
    struct MapTile
    {
        MapTile() = default;
        MapTile(const sf::Vector2f& p, const sf::Angle& r, std::string_view texName, int ty)
            :pos {p}
            ,rotation {r}
            ,textureName {texName}
            ,type {ty}
        {
            static uint32_t idCounter {};
            id = idCounter++;
        }
        
        sf::Vector2f pos;
        sf::Angle rotation;
        std::string textureName;
        int type;
        uint32_t id;
    };
    
    void init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize);
    
    void place(const MapTile& tile);
    void remove(const MapTile& tile);
    
    std::vector<MapTile> getTilesAt(const sf::Vector2f& pos);
    const std::vector<MapTile>& getTiles() { return m_tiles; }
    
    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);
private:
    // takes in a string like "30,42"
    template <typename T>
    sf::Vector2<T> stovec(std::string_view string);
    
    std::vector<MapTile> m_tiles;
    bool hasChanges {false};
    
    sf::Vector2u m_gridSize;
    sf::Vector2f m_worldSize;
};