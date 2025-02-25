#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Grid
{
public:
    struct GridSquare
    {
        GridSquare(sf::Vector2u gp, sf::Vector2f wp, sf::Vector2f mp)
            :gridPos {gp}
            ,worldPos {wp}
            ,midPos {mp}
        {}
        sf::Vector2u gridPos;
        sf::Vector2f worldPos;
        
        sf::Vector2f midPos;
    };
    
    Grid() = default;
    Grid(sf::Vector2u renderSpace, sf::Vector2u squareSize) { init(renderSpace, squareSize); }
    void init(sf::Vector2u renderSpace, sf::Vector2u squareSize);
    
    sf::Vector2u gridToWorld();
    sf::Vector2f worldToGrid();
    
    // could also return a sprite
    sf::RenderTexture drawToTexture();
private:
    std::vector<GridSquare> m_gridCoords;
    // size of the grid in grid squares, rounds up
    sf::Vector2u m_gridSize;
    sf::Vector2f m_worldSize;
    sf::Vector2u m_squareSize;
};