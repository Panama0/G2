#pragma once

#include "SFML/Graphics.hpp"

#include <iostream>

class Animation
{
public:
    Animation() = default;
    Animation(std::string path, uint32_t frames, uint32_t interval)
        :m_framecount {frames}
        ,m_interval {interval}
    {
        if(!loadFromFile(path))
        {
            std::cerr << "Could not load Animation texture!\n";
        }
        m_size = {m_texture.getSize().x / frames, m_texture.getSize().y};
    }
    
    bool loadFromFile(std::string path);
    void updateSprite();
    void addSprite(sf::Sprite* sprite);
    bool hasEnded();
    sf::Vector2u getSize() { return m_size; }
private:
    sf::Sprite* m_sprite;
    sf::Texture m_texture;
    uint32_t m_framecount {};
    uint32_t m_currentGameFrame {};
    uint32_t m_interval {};
    sf::Vector2u m_size {};
};