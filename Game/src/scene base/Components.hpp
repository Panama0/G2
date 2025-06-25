#pragma once

#include "Animation.hpp"
#include "GameMap.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System/Angle.hpp"
#include "SFML/System/Vector2.hpp"
#include <sys/types.h>
#include <vector>

//* surely we can merge animated sprite with this at some point
struct cSprite
{
    cSprite() : sprite{m_defaultTexture} {}
    cSprite(const sf::Texture& tex) : sprite{tex} {}

    sf::Sprite sprite;

private:
    static inline sf::Image m_defaultImg{Vec2u{100, 400},
                                         sf::Color::Magenta};
    static inline sf::Texture m_defaultTexture{m_defaultImg};
};

struct cTransform
{
    cTransform() = default;
    cTransform(Vec2f p, Vec2f v = {0.f, 0.f}) : pos{p}, vel{v} {}
    Vec2f pos;
    Vec2f prevPos;
    Vec2f vel;
    Vec2f scale{1.f, 1.f};
    sf::Angle angle;
};

struct cInput
{
    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
};

struct cBoundingBox
{
    cBoundingBox() = default;
    cBoundingBox(const Vec2f& d) : dimensions{d}, halfDimensions{d / 2.f} {}
    Vec2f dimensions;
    Vec2f halfDimensions;
};

struct cAnimatedSprite
{
    cAnimatedSprite() = default;
    cAnimatedSprite(const Animation& an, bool rep = false)
        : animation{an}, repeat{rep}
    {
    }

    Animation animation;
    bool repeat{false};
};

struct cTileEffect
{
    TileEffect effect;
};

struct cId
{
    uint32_t id{};
};

struct cSpawner
{
    // how long between waves
    int waveTimer{};
    // how often to spawn enemies during a wave
    int waveSpawnInterval{};

    uint32_t enemiesToSpawn{};
};

struct cPathfinder
{
    std::vector<Vec2f>waypoints;
};

struct cTarget
{
    float health{};
};

