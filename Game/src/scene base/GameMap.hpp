#pragma once

#include "Assets.hpp"
#include "Grid.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
#include "TileEffect.hpp"
#include "Vec2.hpp"

#include "SFML/Graphics.hpp"

#include <filesystem>
#include <memory>
#include <unordered_set>
#include <vector>

class GameMap
{
public:
    struct MapTile
    {
        MapTile() = default;
        MapTile(const Vec2i& p, const sf::Angle& r, std::string_view texName)
            : pos{p}, rotation{r}, textureName{texName}
        {
        }

        bool hasEffect(TileEffect::Effects effect)
        {
            return staticEffects.find(effect) != staticEffects.end();
        }

        Vec2i pos;

        sf::Angle rotation;
        std::string textureName;
        std::unordered_set<TileEffect::Effects> staticEffects;
    };

    void init(const Vec2i& mapSize, const Vec2i& tileSize, Assets* assets);

    void placeTile(const Vec2f& pos,
                   const sf::Angle& angle,
                   const std::string& texName);
    void clearTile(const Vec2f& worldPos);

    // add a static effect to a location
    void placeBrush(const TileEffect::Effects& effect, const Vec2f& worldPos);
    void clearBrushes(const Vec2f& worldPos);

    std::shared_ptr<MapTile>& getTile(const Vec2i& pos);
    std::shared_ptr<MapTile>& getTile(const Vec2f& pos);

    const Vec2i& getGridSize() { return m_grid.getSize(); }

    Vec2f toWorldPos(const Vec2i& pos);
    Vec2i toGridPos(const Vec2f& pos);

    bool inBounds(const Vec2i& pos);
    bool inBounds(const Vec2f& pos);

    const std::vector<std::shared_ptr<MapTile>>& getTiles() { return m_tiles; }

    bool save(const std::filesystem::path& path);
    bool load(const std::filesystem::path& path);

    const sf::Texture& getTexture() { return m_renderTexture.getTexture(); }
    void toggleGrid();
    void clear();

private:
    // update the texture that is passed to the game to render
    void updateTexture();

    // TODO: there is a smarter solution out there
    Assets* m_assets;

    Grid m_grid;
    sf::RenderTexture m_renderTexture;
    bool m_showGrid{true};

    // we need mapSize because at some point the map may differ in size to the
    // game world size
    Vec2i m_mapSize;

    std::vector<std::shared_ptr<MapTile>> m_tiles;
    // this is the string we will use to determine if a save is legitimate
    std::string m_identifier{"G2SAVE"};
};
