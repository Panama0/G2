#include "scene base/GameMap.hpp"

#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>

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

[[nodiscard]]
bool GameMap::save(const std::filesystem::path& path)
{
    std::ofstream out {path};

    if(!out)
    {
        std::cerr << "Failed to save game!\n";
        return false;
    }
    
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %X");
    std::string dateString = ss.str();
    
    out << dateString << '\n';
    out << "GridSize: " << m_gridSize.x << "," << m_gridSize.y << '\n';
    out << "WorldSize: " << m_worldSize.x << "," << m_worldSize.y << '\n';
    out << "TileData:\n";
    
    for(const auto& tile : m_tiles)
    {
        out << tile.textureName << ' ' << tile.pos.x << "," << tile.pos.y
            << ' ' << tile.rotation.asRadians() << ' ' << tile.id <<  ' ' << tile.type;
            
        out << '\n';
    }
    return true;
}

[[nodiscard]]
bool GameMap::load(const std::filesystem::path& path)
{
    std::ifstream in {path};
    
    if(!in)
    {
        std::cerr << "Could not load game!\n";
        return false;
    }
    
    m_tiles.clear();
    
    std::string token;
    
    while(in >> token)
    {
        if(token == "GridSize:")
        {
            std::string gridSize;
            in >> gridSize;
            
            m_gridSize = stovec<uint32_t>(gridSize);
        }
        if(token == "WorldSize:")
        {
            std::string worldSize;
            in >> worldSize;
            
            m_worldSize = stovec<float>(worldSize);
        }
        if(token == "TileData:")
        {
            std::string texName;
            std::string posStr;
            std::string rotationStr;
            std::string idStr;
            std::string typeStr;
            
            while(in)
            {
                in >> texName >> posStr >> rotationStr >> idStr >> typeStr;
                
                sf::Vector2f pos {stovec<float>(posStr)};
                sf::Angle angle {sf::radians(std::stof(rotationStr))};
                //! not used for now
                uint32_t id {static_cast<uint32_t>(std::stoi(idStr))};
                int type {std::stoi(typeStr)};
                
                m_tiles.reserve(m_gridSize.x * m_gridSize.y);
                m_tiles.emplace_back(MapTile {pos, angle, texName, type});
            }
        }
    }
    
    return true;
}

template <typename T>
sf::Vector2<T> GameMap::stovec(std::string_view string)
{
    auto midPos {string.find(',')};
    
    if(midPos == std::string::npos)
    {
        std::cerr << "String to vector failed!, Invalid string passed to func.\n";
    }
    
    std::string_view x {string.substr(0, midPos)};
    std::string_view y {string.substr(midPos + 1)};
    
    return sf::Vector2<T> {static_cast<T>(std::stof(x.data())),
                           static_cast<T>(std::stof(y.data()))};
}