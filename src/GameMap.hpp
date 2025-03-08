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
        std::string_view textureName;
        int type;
        uint32_t id;
    };
    
    void place(const MapTile& tile);
    void remove(const MapTile& tile);
    
    std::vector<const MapTile&> getTiles(const sf::Vector2f& pos);
    
    bool save(std::string_view fname);
    bool load(std::string_view fname);
    const sf::Texture& getTexture();
    void setDir(const std::filesystem::path& path);
private:
    void drawToTexture();
    
    std::filesystem::path m_dir;
    std::vector<MapTile> tiles;
    bool hasChanges {false};
    
    sf::RenderTexture m_mapTexture;
};