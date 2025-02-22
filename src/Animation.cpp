#include "Animation.hpp"

bool Animation::loadFromFile(std::string path)
{
    if(!m_texture.loadFromFile(path)) { return false; };
    
    return true;
}

void Animation::updateSprite()
{
    //* temp
    m_sprite->setTexture(m_texture);
    
    uint32_t currentAnimFrame {(m_currentGameFrame / m_interval) % m_framecount};
    
    sf::IntRect renderArea {sf::Vector2i {static_cast<int>(currentAnimFrame * m_size.x), 0}, 
                            static_cast<sf::Vector2i>(m_size)};
    m_sprite->setTextureRect(renderArea);
    
    m_currentGameFrame++;
}