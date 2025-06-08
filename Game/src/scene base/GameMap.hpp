#pragma once

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include "TileEffect.hpp"

#include "SFML/Graphics.hpp"

#include <filesystem>
#include <optional>
#include <vector>

class GameMap
{
public:
    struct MapTile
    {
        MapTile() = default;
        MapTile(const sf::Vector2u& p,
                const sf::Angle& r,
                std::string_view texName)
            : pos{p}, rotation{r}, textureName{texName}
        {
        }

        sf::Vector2u pos;

        sf::Angle rotation;
        std::string textureName;
        std::vector<TileEffect> effects;
    };

    void init(const sf::Vector2u& mapSize, const sf::Vector2u& tileSize)
    {
        m_grid.init(mapSize, tileSize);
    }

    Grid* getGrid() { return &m_grid; }

    void placeTile(const MapTile& tile);
    void removeTile(const sf::Vector2u& pos);

    void placeBrush(const TileEffect& effect, const sf::Vector2u& pos);
    void clearBrushes(const sf::Vector2u& pos);

    std::optional<MapTile> getTileAt(const sf::Vector2f& pos);
    const std::vector<MapTile>& getTiles() { return m_tiles; }

    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);

private:
    void clear();
    MapTile* accessTile(const sf::Vector2u& pos);

    // NOTE: testing for now
    Grid m_grid;

    std::vector<MapTile> m_tiles;

    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier{"G2SAVE"};
};
