#pragma once

#include "SFML/Graphics.hpp"

class Animation
{
public:
    void update();
    bool hasEnded();
    std::string getName();
    sf::Vector2f getSize();
    sf::Sprite& getSprite();
private:
    sf::Sprite m_sprite;
    int m_framecount {};
    int m_currentFrame {};
    int m_interval {};
    sf::Vector2f m_size {};
    sf::Vector2f m_name {};
};