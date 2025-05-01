#include "GameMap.hpp"

#include "Parser.hpp"
#include "Serialisation.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

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

void GameMap::removeEffect(MapTile& tile, uint32_t effectId)
{
    for(auto it = tile.effects.begin(); it != tile.effects.end(); it++)
    {
        if(it->id == effectId)
        {
            tile.effects.erase(it);
            break;
        }
    }
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

[[nodiscard]] bool GameMap::save(const std::filesystem::path& path)
{
    Serialisation save;

    if(!save.begin(path, std::ios::out))
    {
        std::cerr << "Failed to save game!\n";
        return false;
    }

    save.beginSection("MetaData");

    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << m_identifier << ' ';
    ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %X");
    std::string identString = ss.str();

    save.writeLineBuffer(identString);

    save.writeLineBuffer(std::to_string(m_gridSize.x), ",");
    save.writeLineBuffer(std::to_string(m_gridSize.y));

    save.writeLineBuffer(std::to_string(m_worldSize.x));
    save.writeLineBuffer(std::to_string(m_worldSize.y), ",");
    save.endLine();

    save.endSection();

    if(m_tiles.size() > 0)
    {
        save.beginSection("TileData");

        for(const auto& tile : m_tiles)
        {
            save.beginSection("Tile");

            save.writeLineBuffer(tile.textureName);
            save.writeLineBuffer(std::to_string(tile.pos.x), ",");
            save.writeLineBuffer(std::to_string(tile.pos.y));
            save.writeLineBuffer(std::to_string(tile.rotation.asRadians()));
            save.writeLineBuffer(std::to_string(tile.id));
            save.endLine();

            if(!tile.effects.empty())
            {
                save.beginSection("Effects");
                for(const auto& effect : tile.effects)
                {
                    save.writeLineBuffer(effect.textureName);
                    save.writeLineBuffer(TileEffect::toString(effect.effect));
                    save.endLine();
                }
                save.endSection();
            }

            save.endSection();
        }

        save.endSection();
    }

    return true;
}

[[nodiscard]] bool GameMap::load(const std::filesystem::path& path)
{

    Parser p;
    if(!p.begin(path))
    {
        std::cerr << "Could not open save!\n";
    }

    clear();

    auto data = p.parseSave();

    m_worldSize = data.worldSize;
    m_gridSize = data.gridSize;
    m_tiles = data.tiles;

    return true;
}

void GameMap::clear() { m_tiles.clear(); }