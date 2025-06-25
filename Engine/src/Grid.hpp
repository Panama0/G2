#pragma once

#include "SFML/Graphics.hpp"
#include "Vec2.hpp"

#include <vector>

// Generic utility class used for creating grids
class Grid
{
public:
    struct GridSquare
    {
        GridSquare(const Vec2u& gp, const Vec2f& wp, const Vec2f mp)
            : gridPos{gp}, worldPos{wp}, midPos{mp}
        {
        }
        Vec2u gridPos;
        Vec2f worldPos;

        Vec2f midPos;
    };

    Grid() = default;
    Grid(const Vec2u& renderSpace, const Vec2u& squareSize)
    {
        init(renderSpace, squareSize);
    }
    void init(const Vec2u& renderSpace, const Vec2u& squareSize);

    const GridSquare& getGridAt(const Vec2f& worldPos) const;
    const GridSquare& getGridAt(const Vec2u& gridPos) const;

    // get the size of the grid in squares
    const Vec2u& getSize() { return m_gridSize; }
    const Vec2u& getSquareSize() { return m_squareSize; }

    const sf::Texture& getTexture() const
    {
        return m_gridTexture.getTexture();
    }

private:
    void drawToTexture();

    std::vector<GridSquare> m_gridCoords;
    // size of the grid in grid squares, rounds up
    Vec2u m_gridSize;
    Vec2f m_worldSize;
    Vec2u m_squareSize;
    sf::RenderTexture m_gridTexture;
};
