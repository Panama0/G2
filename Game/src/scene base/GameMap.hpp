#pragma once

#include "Assets.hpp"
#include "Grid.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
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

    void init(const sf::Vector2u& mapSize,
              const sf::Vector2u& tileSize,
              Assets* assets);

    void placeTile(const sf::Vector2f& pos,
                   const sf::Angle& angle,
                   const std::string& texName);
    void removeTile(const sf::Vector2f& worldPos);

    void placeBrush(const TileEffect& effect, const sf::Vector2f& worldPos);
    void clearBrushes(const sf::Vector2f& worldPos);

    std::optional<MapTile> getTileAt(const sf::Vector2f& pos);
    sf::Vector2f toWorldPos(const sf::Vector2u& pos);
    sf::Vector2u toGridPos(const sf::Vector2f& pos);

    const std::vector<MapTile>& getTiles() { return m_tiles; }

    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);

    const sf::Texture& getTexture() { return m_renderTexture.getTexture(); }
    void toggleGrid();

private:
    void clear();
    MapTile* accessTile(const sf::Vector2u& pos);

    // update the texture that is passed to the game to render
    void updateTexture();

    // TODO: there is a smarter solution out there
    Assets* m_assets;

    Grid m_grid;
    sf::RenderTexture m_renderTexture;
    bool m_showGrid{true};

    // we need mapSize because at some point the map may differ in size to the
    // game world size
    sf::Vector2u m_mapSize;
    std::vector<MapTile> m_tiles;
    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier{"G2SAVE"};
};
