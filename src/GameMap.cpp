#include "GameMap.hpp"

void GameMap::init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize)
{
    m_gridSize = gridSize;
    m_worldSize = worldSize;
    m_tiles.reserve(gridSize.x * gridSize.y);
}

void GameMap::place(const MapTile& tile)
{
    m_tiles.push_back(tile);
}

void GameMap::remove(const MapTile& tile)
{
    const auto& tileId = tile.id;
    
    for(auto it = m_tiles.begin(); it != m_tiles.end(); it++)
    {
        if(it->id == tileId)
        {
            m_tiles.erase(it);
            break;
        }
    }
}

std::vector<GameMap::MapTile> GameMap::getTilesAt(const sf::Vector2f& pos)
{
    std::vector<GameMap::MapTile> tiles;
    tiles.reserve(5);
    
    for(const auto& tile : m_tiles)
    {
        if(tile.pos == pos)
            tiles.push_back(tile);
    }
    return tiles;
}