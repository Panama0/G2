#include "GameMap.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
#include "SFML/System/Vector2.hpp"
#include "scene base/TileEffect.hpp"
#include "sff/sffParser.hpp"
#include "sff/sffSerialiser.hpp"

#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

void GameMap::init(const Vec2u& mapSize, const Vec2u& tileSize, Assets* assets)
{
    m_grid.init(mapSize, tileSize);
    m_assets = assets;
    m_mapSize = mapSize;

    auto& mapGridSize = m_grid.getSize();
    m_tiles.resize(mapGridSize.x * mapGridSize.y);

    updateTexture();
}

void GameMap::placeTile(const Vec2f& pos,
                        const sf::Angle& angle,
                        const std::string& texName)
{
    auto clickedPos = m_grid.getGridAt(pos).gridPos;

    getTile(pos) = std::make_shared<MapTile>(clickedPos, angle, texName);

    updateTexture();
}

void GameMap::clearTile(const Vec2f& worldPos)
{
    getTile(worldPos) = nullptr;
    updateTexture();
}

void GameMap::placeBrush(const TileEffect& effect, const Vec2f& worldPos)
{
    getTile(worldPos)->effects.push_back(effect);
}

void GameMap::clearBrushes(const Vec2f& worldPos)
{
    getTile(worldPos)->effects.clear();
}

Vec2f GameMap::toWorldPos(const Vec2u& pos)
{
    return m_grid.getGridAt(pos).midPos;
}

Vec2u GameMap::toGridPos(const Vec2f& pos)
{
    return m_grid.getGridAt(pos).gridPos;
}

std::shared_ptr<GameMap::MapTile>& GameMap::getTile(const Vec2u& pos)
{
    return m_tiles[pos.y * m_grid.getSize().x + pos.x];
}

std::shared_ptr<GameMap::MapTile>& GameMap::getTile(const Vec2f& pos)
{
    return getTile(toGridPos(pos));
}

bool GameMap::save(const std::filesystem::path& path)
{
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << m_identifier << ' ';
    ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %X");
    std::string identString = ss.str();

    auto withExtension = path;
    withExtension.replace_extension(".sff");
    sff::Serialiser file{withExtension};

    if(!file.alive())
    {
        return false;
    }

    // MetaData
    file.addNode("MetaData");
    file.addData("Identifier", identString);

    file.addData("WorldSize", std::pair<int, int>{m_mapSize.x, m_mapSize.y});
    auto& squareSize = m_grid.getSquareSize();
    file.addData("SquareSize",
                 std::pair<int, int>{static_cast<uint32_t>(squareSize.x),
                                     static_cast<uint32_t>(squareSize.y)});
    file.endNode();

    // Tile Data
    if(!m_tiles.empty())
    {
        file.addNode("TileData");

        for(const auto& tile : m_tiles)
        {
            if(tile)
            {
                file.addNode("Tile");
                file.addData("TexName", tile->textureName);
                file.addData("Position",
                             std::pair<int, int>{tile->pos.x, tile->pos.y});
                file.addData("Rotation", tile->rotation.asRadians());

                // Effects
                if(!tile->effects.empty())
                {
                    for(const auto& effect : tile->effects)
                    {
                        file.addData(
                            "Effect",
                            std::string{TileEffect::toString(effect.type)});
                    }
                }

                file.endNode();
            }
        }

        file.endNode();
    }

    file.endFile();

    return true;
}

bool GameMap::load(const std::filesystem::path& path)
{
    std::vector<std::shared_ptr<MapTile>> mapTiles;
    Vec2u mapSize;
    Vec2u squareSize;

    sff::Parser file{path};

    if(!file.alive())
    {
        return false;
    }

    auto data = file.parse();

    auto metaData = data->findChild("MetaData");
    if(metaData)
    {
        // TODO: rewrite this with new vector conversions

        std::string ident = metaData->getData("Identifier");
        std::pair<int, int> worldSizeData = metaData->getData("WorldSize");
        std::pair<int, int> squareSizeData = metaData->getData("SquareSize");
        mapSize = {static_cast<uint32_t>(worldSizeData.first),
                   static_cast<uint32_t>(worldSizeData.second)};

        squareSize = {static_cast<uint32_t>(squareSizeData.first),
                      static_cast<uint32_t>(squareSizeData.second)};

        auto& mapGridSize = m_grid.getSize();
        mapTiles.resize(mapGridSize.x * mapGridSize.y);
    }
    else
    {
        // the file is invalid
        return false;
    }

    auto tileData = data->findChild("TileData");
    if(tileData)
    {
        auto& tileNodes = tileData->getChildren();

        for(auto& tileNode : tileNodes)
        {
            std::string tex = tileNode->getData("TexName");
            std::pair<int, int> pos{tileNode->getData("Position")};
            float rotation{tileNode->getData("Rotation")};

            Vec2u tilePosition{static_cast<uint32_t>(pos.first),
                               static_cast<uint32_t>(pos.second)};

            auto mapTile = std::make_shared<MapTile>(
                tilePosition, sf::radians(rotation), tex);

            if(tileNode->hasData("Effect"))
            {
                auto& effects{tileNode->getDataVec("Effect")};

                for(auto& effect : effects)
                {
                    std::string effectName = effect;
                    TileEffect tileEffect{TileEffect::fromString(effectName)};
                    mapTile->effects.push_back(tileEffect);
                }
            }

            mapTiles[pos.second * m_grid.getSize().x + pos.first] = mapTile;
        }
    }

    // we have loaded successfully, its safe to store the tiles
    m_tiles = std::move(mapTiles);

    m_grid.init(m_mapSize, squareSize);

    updateTexture();

    return true;
}

void GameMap::toggleGrid()
{
    m_showGrid ? m_showGrid = false : m_showGrid = true;
    updateTexture();
}

void GameMap::updateTexture()
{
    if(!m_renderTexture.resize(m_mapSize))
    {
        std::cerr << "Could not resize game map texture\n";
    }
    m_renderTexture.clear(sf::Color::Transparent);

    // draw tiles
    for(auto& tile : m_tiles)
    {
        if(tile)
        {
            sf::Texture tex{m_assets->getTexture(tile->textureName)};
            sf::Sprite spr{tex};
            spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
            spr.setPosition(m_grid.getGridAt(tile->pos).midPos);

            m_renderTexture.draw(spr);
        }
    }

    // draw grid
    if(m_showGrid)
    {
        sf::Sprite grid{m_grid.getTexture()};
        m_renderTexture.draw(grid);
    }

    m_renderTexture.display();
}

void GameMap::clear() { m_tiles.clear(); }
