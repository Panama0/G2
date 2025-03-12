#include "GameMap.hpp"

void GameMap::init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize, Assets* assets)
{
    m_gridSize = gridSize;
    m_worldSize = worldSize;
    m_tiles.reserve(gridSize.x * gridSize.y);
    m_assets = assets;
}

void GameMap::update()
{
    drawToTexture();
}

void GameMap::drawToTexture()
{
    m_mapTexture.clear();
    for(const auto& tile : m_tiles)
    {
        sf::Texture tex {m_assets->getTexture(tile.textureName)};
        sf::Sprite spr {tex};
        spr.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
        spr.setPosition(tile.pos);
        spr.setRotation(tile.rotation);
        
        m_mapTexture.draw(spr);
    }
    m_mapTexture.display();
}

void GameMap::place(const MapTile& tile)
{
    m_tiles.push_back(tile);
}

void GameMap::remove(const MapTile& tile)
{
    auto& tileId = tile.id;
    
    for(auto it = m_tiles.begin(); it != m_tiles.end(); it++)
    {
        if(it->id == tileId)
        {
            m_tiles.erase(it);
        }
    }
}

std::vector<GameMap::MapTile> GameMap::getTilesAt(const sf::Vector2f& pos)
{
    std::vector<GameMap::MapTile> tiles(5);
    
    for(const auto& tile : m_tiles)
    {
        tiles.push_back(tile);
    }
    return tiles;
}