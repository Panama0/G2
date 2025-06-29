#include "Grid.hpp"

#include <iostream>

void Grid::init(const Vec2i& renderSpace, const Vec2i& squareSize)
{
    m_worldSize = static_cast<Vec2f>(renderSpace);
    // integer division
    m_gridSize = {(renderSpace.x + squareSize.x - 1) / squareSize.x,
                  (renderSpace.y + squareSize.y - 1) / squareSize.y};
    m_squareSize = squareSize;
    // set up the grid texture
    if(!m_gridTexture.resize(static_cast<Vec2u>(renderSpace)))
    {
        std::cerr << "Could not resize!\n";
    }

    // load the coords into the vector
    m_gridCoords.reserve(m_gridSize.x * m_gridSize.y);
    for(int y{}; y < m_gridSize.y; y++)
    {
        for(int x{}; x < m_gridSize.x; x++)
        {
            Vec2i gp{x % m_gridSize.x, y % m_gridSize.y};
            Vec2f wp{static_cast<float>(x * m_squareSize.x),
                     static_cast<float>(y * m_squareSize.y)};
            Vec2f mp{wp.x + (m_squareSize.x / 2.f),
                     wp.y + (m_squareSize.y / 2.f)};

            m_gridCoords.emplace_back(gp, wp, mp);
        }
    }
    drawToTexture();
}

const Grid::GridSquare& Grid::getGridAt(const Vec2f& worldPos) const
{
    assert(worldPos.x >= 0 && worldPos.y >= 0 && "worldPos must be positive!");
    assert(worldPos.x < m_worldSize.x && worldPos.y < m_worldSize.y
           && "worldPos cant be larger than world!");
    Vec2i gp{static_cast<int>(worldPos.x) / m_squareSize.x,
             static_cast<int>(worldPos.y) / m_squareSize.y};
    return getGridAt(gp);
}

const Grid::GridSquare& Grid::getGridAt(const Vec2i& gridPos) const
{
    return m_gridCoords[gridPos.y * m_gridSize.x + gridPos.x];
}

void Grid::drawToTexture()
{
    sf::Color col{sf::Color::White};
    float thickness{1.5f};
    m_gridTexture.clear(sf::Color::Transparent);
    // vertical
    for(int i{}; i < m_gridSize.x + 1; i++)
    {
        sf::VertexArray vLine{sf::PrimitiveType::TriangleStrip, 6};

        // the below points are clockwise around a rectangle.
        sf::Vertex a{
            {static_cast<float>(m_squareSize.x * i - thickness / 2.f), 0.f},
            col};
        sf::Vertex b{{a.position.x + thickness, 0.f}, col};
        sf::Vertex c{{b.position.x, m_worldSize.y}, col};
        sf::Vertex d{{a.position.x, m_worldSize.y}, col};
        // triangle 1
        vLine[0] = (a);
        vLine[1] = (b);
        vLine[2] = (c);
        // triangle 2
        vLine[3] = (c);
        vLine[4] = (d);
        vLine[5] = (a);

        m_gridTexture.draw(vLine);
    }
    // horizontal
    for(size_t i{}; i < m_gridSize.y; i++)
    {
        sf::VertexArray hLine{sf::PrimitiveType::TriangleStrip, 6};

        sf::Vertex a{
            {0.f, static_cast<float>(m_squareSize.y * i - thickness / 2.f)},
            col};
        sf::Vertex b{{m_worldSize.x, a.position.y}, col};
        sf::Vertex c{{b.position.x, b.position.y + thickness}, col};
        sf::Vertex d{{a.position.x, c.position.y}, col};

        // triangle 1
        hLine[0] = (a);
        hLine[1] = (b);
        hLine[2] = (c);
        // triangle 2
        hLine[3] = (c);
        hLine[4] = (d);
        hLine[5] = (a);

        m_gridTexture.draw(hLine);
    }
    m_gridTexture.display();
}
