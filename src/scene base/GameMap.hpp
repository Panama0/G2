#pragma once

#include "SFML/Graphics.hpp"

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
        enum BrushTypes
        {
            water,
            obstructed,
            path,
            spawner,
            
            
            
            count,
        };
        
        static constexpr std::string_view toString(BrushTypes brush)
        {
            switch(brush)
            {
                case BrushTypes::water:
                    return "Water";
                case BrushTypes::obstructed:
                    return "Obstructed";
                case BrushTypes::path:
                    return "Path";
                case BrushTypes::spawner:
                    return "Spawner";
                default:
                    return "Invalid\n";
            }
        }
        
        Brush() = default;
        Brush(const sf::Vector2f& p, const sf::Angle& r, std::string_view texName, BrushTypes ty)
            :MapTile(p, r, texName)
            ,type {ty}
            {}
        
        BrushTypes type;
        
    };
    
    void init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize);
    
    void placeTile(const MapTile& tile);
    void removeTile(uint32_t id);
    void placeBrush(const Brush& brush);
    void removeBrush(uint32_t id);
    
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