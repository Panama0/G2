#pragma once

#include "Assets.hpp"
#include "Grid.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
#include "Vec2.hpp"
#include "TileEffect.hpp"

#include "SFML/Graphics.hpp"

#include <filesystem>
#include <memory>
#include <vector>

class GameMap
{
public:
    struct MapTile
    {
        MapTile() = default;
        MapTile(const Vec2u& p,
                const sf::Angle& r,
                std::string_view texName)
            : pos{p}, rotation{r}, textureName{texName}
        {
        }

        Vec2u pos;

        sf::Angle rotation;
        std::string textureName;
        std::vector<TileEffect> effects;
    };

    void init(const Vec2u& mapSize,
              const Vec2u& tileSize,
              Assets* assets);

    void placeTile(const Vec2f& pos,
                   const sf::Angle& angle,
                   const std::string& texName);
    void clearTile(const Vec2f& worldPos);

    void placeBrush(const TileEffect& effect, const Vec2f& worldPos);
    void clearBrushes(const Vec2f& worldPos);

    std::shared_ptr<MapTile>& getTile(const Vec2u& pos);
    std::shared_ptr<MapTile>& getTile(const Vec2f& pos);

    Vec2f toWorldPos(const Vec2u& pos);
    Vec2u toGridPos(const Vec2f& pos);

    const std::vector<std::shared_ptr<MapTile>>& getTiles() { return m_tiles; }

    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);

    const sf::Texture& getTexture() { return m_renderTexture.getTexture(); }
    void toggleGrid();

private:
    void clear();

    // update the texture that is passed to the game to render
    void updateTexture();

    // TODO: there is a smarter solution out there
    Assets* m_assets;

    Grid m_grid;
    sf::RenderTexture m_renderTexture;
    bool m_showGrid{true};

    // we need mapSize because at some point the map may differ in size to the
    // game world size
    Vec2u m_mapSize;

    std::vector<std::shared_ptr<MapTile>> m_tiles;
    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier{"G2SAVE"};
};
