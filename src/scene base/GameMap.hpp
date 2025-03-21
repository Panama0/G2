#pragma once

#include "SFML/Graphics.hpp"

#include "scene base/GameEnums.hpp"

#include <filesystem>
#include <vector>

class GameMap
{
public:
    struct MapTile
    {
        MapTile() = default;
        MapTile(const sf::Vector2f& p, const sf::Angle& r, std::string_view texName)
            :pos {p}
            ,rotation {r}
            ,textureName {texName}
            ,id {m_idCounter++}
        {}
        
        sf::Vector2f pos;
        sf::Angle rotation;
        std::string textureName;
        uint32_t id {};
    
    private:
        static inline uint32_t m_idCounter {};
    };
    
    struct Brush : public MapTile
    {
        Brush() = default;
        Brush(const sf::Vector2f& p, const sf::Angle& r, std::string_view texName, int ty)
            :MapTile(p, r, texName)
            ,type {ty}
            {}
        
        enums::Brushes type;
    };
    
    void init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize);
    
    void place(const MapTile& tile);
    void remove(const MapTile& tile);
    
    std::vector<MapTile> getTilesAt(const sf::Vector2f& pos);
    const std::vector<MapTile>& getTiles() { return m_tiles; }
    
    std::vector<Brush> getBrushesAt(const sf::Vector2f& pos);
    const std::vector<Brush>& getBrushes() { return m_brushes; }
    
    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);
private:
    // takes in a string like "30,42"
    template <typename T>
    sf::Vector2<T> stovec(std::string_view string);
    
    std::vector<MapTile> m_tiles;
    std::vector<Brush> m_brushes;
    
    sf::Vector2u m_gridSize;
    sf::Vector2f m_worldSize;
    
    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier {"G2SAVE"};
};