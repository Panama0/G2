#pragma once

#include "SFML/Graphics.hpp"

class Animation
{
public:
    void update();
    bool hasEnded();
    std::string getName() { return m_name; }
    sf::Vector2f getSize() { return m_size; }
    sf::Sprite& getSprite() {return m_sprite; }
private:
    sf::Sprite m_sprite;
    int m_framecount {};
    int m_currentFrame {};
    int m_interval {};
    sf::Vector2f m_size {};
    std::string m_name {};
};