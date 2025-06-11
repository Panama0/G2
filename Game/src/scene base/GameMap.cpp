#include "GameMap.hpp"

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
#include "SFML/System/Vector2.hpp"
#include "scene base/TileEffect.hpp"
#include "sff/sffParser.hpp"
#include "sff/sffSerialiser.hpp"

#include <chrono>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

void GameMap::init(const sf::Vector2u& mapSize,
                   const sf::Vector2u& tileSize,
                   Assets* assets)
{
    m_grid.init(mapSize, tileSize);
    m_assets = assets;
    m_mapSize = mapSize;
    updateTexture();
}

void GameMap::placeTile(const sf::Vector2f& pos,
                        const sf::Angle& angle,
                        const std::string& texName)
{
    GameMap::MapTile tile{m_grid.getGridAt(pos).gridPos, angle, texName};
    // remove the existing tile if there is one
    if(accessTile(tile.pos))
    {
        removeTile(m_grid.getGridAt(tile.pos).worldPos);
    }

    m_tiles.push_back(tile);
    updateTexture();
}

void GameMap::removeTile(const sf::Vector2f& worldPos)
{
    for(auto it = m_tiles.begin(); it != m_tiles.end(); it++)
    {
        if(it->pos == m_grid.getGridAt(worldPos).gridPos)
        {
            m_tiles.erase(it);
            break;
        }
    }
    updateTexture();
}

void GameMap::placeBrush(const TileEffect& effect,
                         const sf::Vector2f& worldPos)
{
    accessTile(m_grid.getGridAt(worldPos).gridPos)->effects.push_back(effect);
}

void GameMap::clearBrushes(const sf::Vector2f& worldPos)
{
    accessTile(m_grid.getGridAt(worldPos).gridPos)->effects.clear();
}

std::optional<GameMap::MapTile> GameMap::getTileAt(const sf::Vector2f& pos)
{
    for(const auto& tile : m_tiles)
    {
        if(tile.pos == m_grid.getGridAt(pos).gridPos)
        {
            return tile;
        }
    }
    // there was no tile with the same position
    return std::nullopt;
}

sf::Vector2f GameMap::toWorldPos(const sf::Vector2u& pos)
{
    return m_grid.getGridAt(pos).midPos;
}

GameMap::MapTile* GameMap::accessTile(const sf::Vector2u& pos)
{
    for(auto& tile : m_tiles)
    {
        if(tile.pos == pos)
        {
            return &tile;
        }
    }
    // there was no tile with the same position
    return nullptr;
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
    file.endNode();

    // Tile Data
    if(!m_tiles.empty())
    {
        file.addNode("TileData");

        for(const auto& tile : m_tiles)
        {
            file.addNode("Tile");
            file.addData("TexName", tile.textureName);
            file.addData("Position",
                         std::pair<int, int>{tile.pos.x, tile.pos.y});
            file.addData("Rotation", tile.rotation.asRadians());

            // Effects
            if(!tile.effects.empty())
            {
                for(const auto& effect : tile.effects)
                {
                    file.addData(
                        "Effect",
                        std::string{TileEffect::toString(effect.type)});
                }
            }

            file.endNode();
        }

        file.endNode();
    }

    file.endFile();

    return true;
}

bool GameMap::load(const std::filesystem::path& path)
{
    std::vector<GameMap::MapTile> mapTiles;
    sff::Parser file{path};

    if(!file.alive())
    {
        return false;
    }

    auto data = file.parse();

    auto metaData = data->findChild("MetaData");
    if(metaData)
    {
        // TODO: use metadata
        std::string ident = metaData->getData("Identifier");
    }
    else
    {
        // the file is invalid
        return false;
    }

    auto tileData = data->findChild("TileData");
    if(tileData)
    {
        auto& tiles = tileData->getChildren();

        for(auto& tile : tiles)
        {
            std::string tex = tile->getData("TexName");
            std::pair<int, int> pos{tile->getData("Position")};
            float rotation{tile->getData("Rotation")};

            GameMap::MapTile mapTile{{static_cast<uint32_t>(pos.first),
                                      static_cast<uint32_t>(pos.second)},
                                     sf::radians(rotation),
                                     tex};

            if(tile->hasData("Effect"))
            {
                auto& effects{tile->getDataVec("Effect")};

                for(auto& effect : effects)
                {
                    std::string effectName = effect;
                    TileEffect tileEffect{TileEffect::fromString(effectName)};
                    mapTile.effects.push_back(tileEffect);
                }
            }
            mapTiles.push_back(mapTile);
        }
    }

    // we have loaded successfully, its safe to store the tiles
    m_tiles = std::move(mapTiles);
    updateTexture();
    return true;
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
        sf::Texture tex{m_assets->getTexture(tile.textureName)};
        sf::Sprite spr{tex};
        spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
        spr.setPosition(m_grid.getGridAt(tile.pos).midPos);

        m_renderTexture.draw(spr);
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
