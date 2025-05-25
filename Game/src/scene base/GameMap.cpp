#include "GameMap.hpp"

#include "SFML/System/Angle.hpp"
#include "scene base/TileEffect.hpp"
#include "sff/sffParser.hpp"
#include "sff/sffSerialiser.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include <string>

void GameMap::init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize)
{
    m_gridSize = gridSize;
    m_worldSize = worldSize;
    m_tiles.reserve(gridSize.x * gridSize.y);
}

void GameMap::placeTile(const MapTile& tile) { m_tiles.push_back(tile); }

void GameMap::removeTile(uint32_t id)
{
    for(auto it = m_tiles.begin(); it != m_tiles.end(); it++)
    {
        if(it->id == id)
        {
            m_tiles.erase(it);
            break;
        }
    }
}

void GameMap::placeEffect(uint32_t tileId, const TileEffect& effect)
{
    for(auto& tile : m_tiles)
    {
        if(tile.id == tileId)
        {
            tile.effects.emplace_back(effect);
        }
    }
}

// Removes all effects from a tile
void GameMap::removeEffect(MapTile& tile, uint32_t effectId)
{
    tile.effects.clear();
}

std::optional<GameMap::MapTile> GameMap::getTileAt(const sf::Vector2f& pos)
{
    for(const auto& tile : m_tiles)
    {
        if(tile.pos == pos)
        {
            return tile;
        }
    }
    // there was no tile with the same position
    return std::nullopt;
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
    file.addData("GridSize", std::pair<int, int>{m_gridSize.x, m_gridSize.y});
    file.addData("WorldSize",
                 std::pair<int, int>{m_worldSize.x, m_worldSize.y});
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
                         std::pair<float, float>{tile.pos.x, tile.pos.y});
            file.addData("Rotation", tile.rotation.asRadians());
            file.addData("ID", static_cast<int>(tile.id));

            // Effects
            if(!tile.effects.empty())
            {
                for(const auto& effect : tile.effects)
                {
                    file.addData(
                        "Effect",
                        std::string{TileEffect::toString(effect.effect)});
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
    // TODO: The file extension should be added elsewhere
    auto withExtension = path;
    withExtension.replace_extension(".sff");
    sff::Parser file{withExtension};

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
            int id {tile->getData("ID")};
            std::pair<float, float> pos {tile->getData("Position")};
            float rotation{tile->getData("Rotation")};

            GameMap::MapTile mapTile {{pos.first, pos.second},sf::radians(rotation),tex};

            if(tile->hasData("Effect"))
            {
                auto& effects{tile->getDataVec("Effect")};

                for(auto& effect : effects)
                {
                    std::string effectName = effect;
                    TileEffect tileEffect {TileEffect::fromString(effectName)};
                    mapTile.effects.push_back(tileEffect);
                }
            }
            mapTiles.push_back(mapTile);
        }
    }

    // we have loaded successfully, its safe to store the tiles
    m_tiles = std::move(mapTiles);
    return true;
}

void GameMap::clear() { m_tiles.clear(); }
