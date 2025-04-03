#include "scene base/GameMap.hpp"

#include "engine/Serialisation.hpp"

#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include <optional>

void GameMap::init(const sf::Vector2u& gridSize, const sf::Vector2f& worldSize)
{
    m_gridSize = gridSize;
    m_worldSize = worldSize;
    m_tiles.reserve(gridSize.x * gridSize.y);
}

void GameMap::placeTile(const MapTile& tile)
{
    m_tiles.push_back(tile);
}

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
    Serialisation outS;

    if(!outS.begin(path, std::ios::out))
    {
        std::cerr << "Failed to save game!\n";
        return false;
    }
    

    outS.beginSection("MetaData");
    
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << m_identifier << ' ';
    ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %X");
    std::string identString = ss.str();
    
    outS.writeLine(identString);
    
    outS.writeLineBuffer("GridSize:");
    outS.writeLineBuffer(std::to_string(m_gridSize.x), ",");
    outS.writeLineBuffer(std::to_string(m_gridSize.y));
    outS.endLine();
    
    outS.writeLineBuffer("WorldSize:");
    outS.writeLineBuffer(std::to_string(m_worldSize.x));
    outS.writeLineBuffer(std::to_string(m_worldSize.y));
    outS.endLine();
    
    outS.endSection();
    
    if(m_tiles.size() > 0)
    {
        outS.beginSection("TileData");

        for(const auto& tile : m_tiles)
        {
            outS.beginSection("Tile");
            
            outS.writeLineBuffer(tile.textureName);
            outS.writeLineBuffer(std::to_string(tile.pos.x), ",");
            outS.writeLineBuffer(std::to_string(tile.pos.y));
            outS.writeLineBuffer(std::to_string(tile.rotation.asRadians()));
            outS.writeLineBuffer(std::to_string(tile.id));
            outS.endLine();
            
            if (!tile.effects.empty())
            {
                outS.beginSection("Effects");
                for(const auto& effect : tile.effects)
                {
                    outS.writeLineBuffer(effect.textureName);
                    outS.writeLineBuffer(TileEffect::toString(effect.effect));
                    outS.endLine();
                }
                outS.endSection();
            }
            
            outS.endSection();
        }

        outS.endSection();
    }
    
    return true;
}

[[nodiscard]] bool GameMap::load(const std::filesystem::path& path)
{
    std::ifstream in {path};
    
    if(!in)
    {
        std::cerr << "Could not load game!\n";
        return false;
    }
    
    clear();
    
    std::string token;
    std::string buffer;
    

    // in >> token;
    
    // if(token != m_identifier)
    // {
    //     std::cerr << "Not a valid save file\n";
    //     return false;
    // }
    
    // while(in >> token)
    // {
    //     if(token == "GridSize:")
    //     {
    //         std::string gridSize;
    //         in >> gridSize;
            
    //         m_gridSize = stovec<uint32_t>(gridSize);
    //     }
    //     if(token == "WorldSize:")
    //     {
    //         std::string worldSize;
    //         in >> worldSize;
            
    //         m_worldSize = stovec<float>(worldSize);
    //     }
    //     if(token == "TileData:")
    //     {
    //         std::string texName;
    //         std::string posStr;
    //         std::string rotationStr;
    //         std::string idStr;
            
            
    //         while(token != "EndTileData" && in)
    //         {
    //             in >> token;
                
    //             if(token != "Tile")
    //             {
    //                 std::cerr << "Expected tile!\n";
    //             }
                
    //             in >> texName >> posStr >> rotationStr >> idStr;
                
    //             sf::Vector2f pos {stovec<float>(posStr)};
    //             sf::Angle angle {sf::radians(std::stof(rotationStr))};
    //             //uint32_t id {static_cast<uint32_t>(std::stoi(idStr))};
                
    //             MapTile  tile {pos, angle, texName};
    //             auto tileId = tile.id;
                
    //             m_tiles.emplace_back(tile);
                
    //             in >> token;
    //             if(token == "Effects:")
    //             {
    //                 std::string effectStr;
    //                 while(effectStr != "EndTile" && in)
    //                 {
    //                     in >> effectStr;
                        
    //                     placeEffect(tileId, TileEffect::fromString(effectStr));
    //                 }
    //             }
    //             else
    //             {
    //                 std::cerr << "Effects block expected!\n";
    //             }
    //             //in >> token;
    //         }
    //     }
    // }
    
    return true;
}

template <typename T>
sf::Vector2<T> GameMap::stovec(std::string_view string)
{
    auto midPos {string.find(',')};
    
    if(midPos == std::string::npos)
    {
        std::cerr << "String to vector failed!, Invalid string passed to func.\n";
        return sf::Vector2<T> {};
    }
    
    std::string_view x {string.substr(0, midPos)};
    std::string_view y {string.substr(midPos + 1)};
    
    return sf::Vector2<T> {static_cast<T>(std::stof(x.data())),
                           static_cast<T>(std::stof(y.data()))};
}

void GameMap::clear()
{
    m_tiles.clear();
}