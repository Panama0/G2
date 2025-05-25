#pragma once

#include "SFML/Graphics.hpp"
#include "Animation.hpp"
#include "GameMap.hpp"

//* surely we can merge animated sprite with this at some point
struct cSprite
{
    cSprite() : sprite{m_defaultTexture} {}
    cSprite(const sf::Texture& tex) : sprite{tex} {}

    sf::Sprite sprite;

private:
    static inline sf::Image m_defaultImg{sf::Vector2u{100, 400},
                                         sf::Color::Magenta};
    static inline sf::Texture m_defaultTexture{m_defaultImg};
};

struct cTransform
{
    cTransform() = default;
    cTransform(sf::Vector2f p, sf::Vector2f v = {0.f, 0.f}) : pos{p}, vel{v} {}
    sf::Vector2f pos;
    sf::Vector2f prevPos;
    sf::Vector2f vel;
    sf::Vector2f scale{1.f, 1.f};
    float angle;
};

struct cInput
{
    cInput() = default;

    bool up{false};
    bool down{false};
    bool left{false};
    bool right{false};
};

struct cBoundingBox
{
    cBoundingBox() = default;
    cBoundingBox(sf::Vector2f d) : dimensions{d}, halfDimensions{d / 2.f} {}
    sf::Vector2f dimensions;
    sf::Vector2f halfDimensions;
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

struct cAnimation
{
    cAnimation() = default;
    cAnimation(const Animation& an, bool rep = false)
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
    cId() = default;
    cId(uint32_t _id) : id{_id} {}

    uint32_t id{};
};
