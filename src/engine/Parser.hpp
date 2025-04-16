#pragma once

#include "scene base/GameMap.hpp"

#include "SFML/System/Vector2.hpp"

#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
#include <stack>
#include <vector>

#include <iostream>

struct SaveData
{
    // metadata
    sf::Vector2u gridSize;
    sf::Vector2f worldSize;
    // tiles
    std::vector<GameMap::MapTile> tiles;
};

struct Node
{
    virtual void parseLine(const std::string& line) {};
    virtual void store(SaveData* data) {};
    virtual void validate() {};

    template <typename T> sf::Vector2<T> stovec(std::string_view string)
    {
        auto midPos{string.find(',')};

        if(midPos == std::string::npos)
        {
            std::cerr << "String to vector failed!, Invalid string passed to "
                         "func.\n";
            return sf::Vector2<T>{};
        }

        std::string_view x{string.substr(0, midPos)};
        std::string_view y{string.substr(midPos + 1)};

        return sf::Vector2<T>{static_cast<T>(std::stof(x.data())),
                              static_cast<T>(std::stof(y.data()))};
    }
};

struct NodeEffect : public Node
{
    std::string texName;
    std::string effectName;

    void parseLine(const std::string& line) override
    {
        std::stringstream data{line};

        data >> effectName;
    }

    void store(SaveData* data) override
    {
        auto effect = TileEffect::fromString(effectName);
        data->tiles.back().effects.emplace_back(effect);
    }
};

struct NodeTile : public Node
{
    std::string texName;
    std::string pos;
    std::string rotation;
    std::string id;

    std::vector<NodeEffect> effects;

    void parseLine(const std::string& line) override
    {
        std::stringstream data{line};

        data >> texName >> pos >> rotation >> id;
    }

    void store(SaveData* data) override
    {
        sf::Vector2f position = stovec<float>(pos);
        sf::Angle angle = sf::radians(std::stof(rotation));

        data->tiles.back() = {position, angle, texName};
    }
};

struct NodeTileData : public Node
{
    std::vector<NodeTile> tiles;
};

struct NodeMetaData : public Node
{
    std::string identifier;
    std::string timeStamp;
    std::string gridSize;
    std::string worldSize;

    void parseLine(const std::string& line) override
    {
        std::stringstream data{line};

        data >> identifier;
        data >> timeStamp >> timeStamp;
        data >> gridSize >> worldSize;
    }

    void store(SaveData* data) override
    {
        data->gridSize = stovec<uint32_t>(gridSize);
        data->worldSize = stovec<float>(worldSize);
    }

    std::string tag{"MetaData"};
};

class Parser
{
public:
    bool begin(const std::filesystem::path& path)
    {
        m_file.open(path, std::ios::in);

        return m_file.is_open();
    }

    SaveData parseSave()
    {
        std::string token;
        while(m_file >> token)
        {
            if(token == "MetaData")
            {
                m_nodes.push(std::make_shared<NodeMetaData>());
                continue;
            }
            else if(token == "TileData")
            {
                m_nodes.push(std::make_shared<NodeTileData>());
                continue;
            }
            else if(token == "Tile")
            {
                m_nodes.push(std::make_shared<NodeTile>());
                m_saveData.tiles.emplace_back();
                continue;
            }
            else if(token == "Effects")
            {
                m_nodes.push(std::make_shared<NodeEffect>());
                continue;
            }
            else if(token.find("End") != std::string::npos)
            {
                m_nodes.pop();
                continue;
            }
            else // we can parse the text
            {
                std::string line;
                std::getline(m_file, line);

                m_nodes.top()->parseLine(line.insert(0, token));
                m_nodes.top()->validate();
                m_nodes.top()->store(&m_saveData);
            }
        }

        return m_saveData;
    }

private:
    std::stack<std::shared_ptr<Node> > m_nodes;
    std::fstream m_file;

    SaveData m_saveData;
};