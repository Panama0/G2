#pragma once

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
        MapTile(const sf::Vector2f& p,
                const sf::Angle& r,
                std::string_view texName)
            : worldPos{p}, rotation{r}, textureName{texName}, id{m_idCounter++}
        {
        }

        sf::Vector2f worldPos;
        sf::Angle rotation;
        std::string textureName;
        uint32_t id{};
        std::vector<TileEffect> effects;

    private:
        static inline uint32_t m_idCounter{};
    };

    void init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize);

    void placeTile(const MapTile& tile);
    void removeTile(uint32_t id);
    void placeEffect(uint32_t tileId, const TileEffect& effect);
    void removeEffect(MapTile& tile, uint32_t id);

    std::optional<MapTile> getTileAt(const sf::Vector2f& pos);
    const std::vector<MapTile>& getTiles() { return m_tiles; }

    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);

private:
    void clear();

    std::vector<MapTile> m_tiles;

    sf::Vector2u m_gridSize;
    sf::Vector2f m_worldSize;

    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier{"G2SAVE"};
};
