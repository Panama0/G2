#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/Vector2.hpp"

#include <vector>

// Generic utility class used for creating grids
class Grid
{
public:
    struct GridSquare
    {
        GridSquare(sf::Vector2u gp, sf::Vector2f wp, sf::Vector2f mp)
            : gridPos{gp}, worldPos{wp}, midPos{mp}
        {
        }
        sf::Vector2u gridPos;
        sf::Vector2f worldPos;

        sf::Vector2f midPos;
    };

    Grid() = default;
    Grid(sf::Vector2u renderSpace, sf::Vector2u squareSize)
    {
        init(renderSpace, squareSize);
    }
    void init(sf::Vector2u renderSpace, sf::Vector2u squareSize);

    const GridSquare& getGridAt(sf::Vector2f worldPos) const;
    const GridSquare& getGridAt(sf::Vector2u gridPos) const;

    // get the size of the grid in squares
    const sf::Vector2u& getSize() { return m_gridSize; }
    const sf::Vector2u& getSquareSize() { return m_squareSize; }

    const sf::Texture& getTexture() const
    {
        return m_gridTexture.getTexture();
    }

private:
    void drawToTexture();

    std::vector<GridSquare> m_gridCoords;
    // size of the grid in grid squares, rounds up
    sf::Vector2u m_gridSize;
    sf::Vector2f m_worldSize;
    sf::Vector2u m_squareSize;
    sf::RenderTexture m_gridTexture;
};
