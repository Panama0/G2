#include "Grid.hpp"

#include <cmath>
#include <iostream>

void Grid::init(sf::Vector2u renderSpace, sf::Vector2u squareSize)
{
    m_worldSize = static_cast<sf::Vector2f>(renderSpace);
    // integer division
    m_gridSize = {(renderSpace.x + squareSize.x - 1) / squareSize.x,
                  (renderSpace.y + squareSize.y - 1) / squareSize.y};
    m_squareSize = squareSize;
    
    // load the coords into the vector
    for(uint32_t y {}; y < m_gridSize.y; y++)
    {
        for(uint32_t x {}; x < m_gridSize.x; x++)
        {
            sf::Vector2u gp {x % m_gridSize.x, y % m_gridSize.y};
            sf::Vector2f wp {static_cast<float>(x * m_squareSize.x),
                             static_cast<float>(y * m_squareSize.y)};
            sf::Vector2f mp {wp.x + (m_squareSize.x / 2.f), wp.y + (m_squareSize.y / 2.f)};
            
            m_gridCoords.emplace_back(gp, wp, mp);
        }
    }
}

const Grid::GridSquare& Grid::getGridAt(sf::Vector2f worldPos) const
{
    assert(worldPos.x < m_worldSize.x && worldPos.y < m_worldSize.y && 
            "worldPos cant be larger than world!\n");
    sf::Vector2u gp {static_cast<uint32_t>(worldPos.x) / m_squareSize.x,
                     static_cast<uint32_t>(worldPos.y) / m_squareSize.y};
    return getGridAt(gp);
}

const Grid::GridSquare& Grid::getGridAt(sf::Vector2u gridPos) const
{
    return m_gridCoords[gridPos.y * m_gridSize.x + gridPos.x];
}