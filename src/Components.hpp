#pragma once

#include "Animation.hpp"
#include "SFML/Graphics.hpp"


class Component
{
public:
    bool exists {false};
};

//* surely we can merge animated sprite with this at some point
class cSprite : public Component
{
public:
    cSprite() = default;
    cSprite(const sf::Texture& tex)
        :sprite {tex}
    {}
    
    // default texture
    sf::Image def {sf::Vector2u{100, 100}, sf::Color::Magenta};
    
    sf::Texture texture {def};
    sf::Sprite sprite{texture};
};

class cTransform : public Component
{
public:
    cTransform() = default;
    cTransform(sf::Vector2f p, sf::Vector2f v = {0.f, 0.f})
        :pos{p}
        ,vel{v}
    {}
    sf::Vector2f pos;
    sf::Vector2f prevPos;
    sf::Vector2f vel;
    sf::Vector2f scale {1.f, 1.f};
    float angle;
};

class cInput : public Component
{
public:
    cInput() = default;
    
    bool up {false};
    bool down {false};
    bool left {false};
    bool right {false};
    //* can add more here if needed
};

class cBoundingBox : public Component
{
public:
    cBoundingBox() = default;
    cBoundingBox(sf::Vector2f d)
        :dimensions {d}
        ,halfDimensions {d/2.f}
    {}
    sf::Vector2f dimensions;
    sf::Vector2f halfDimensions;
};

class cAnimatedSprite : public Component
{
public:
    cAnimatedSprite() = default;
    cAnimatedSprite(const Animation& an, bool rep = false)
        :animation {an}
        ,repeat {rep}
    {}
    
    Animation animation;
    bool repeat {false};
};