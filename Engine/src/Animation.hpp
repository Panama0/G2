#pragma once

#include "SFML/Graphics.hpp"
#include "Vec2.hpp"

#include <iostream>

class Animation
{
public:
    Animation() = default;
    Animation(std::string path, uint32_t frames, uint32_t interval)
        : isValid{true}, m_framecount{frames}, m_interval{interval}
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
    Vec2u getSize() { return m_size; }
    bool isValid{false};

private:
    sf::Sprite* m_sprite;
    sf::Texture m_texture;
    uint32_t m_framecount{};
    uint32_t m_currentGameFrame{};
    uint32_t m_interval{};
    Vec2u m_size{};
};
