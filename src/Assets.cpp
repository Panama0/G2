#include "Assets.hpp"

bool Assets::addTexture(std::string name, std::string path)
{
    sf::Texture texture;
    if(!texture.loadFromFile(path)) { return false; }
    m_textures.emplace(name, texture);
    return true;
}

bool Assets::addAnimation(std::string name, std::string path, uint32_t frames, uint32_t interval)
{
    Animation animation {path, frames, interval};
    
    m_animations.emplace(name, animation);
    //* temp
    return true;
}

bool Assets::addSound(std::string name, std::string path)
{
    
}

bool Assets::addFont(std::string name, std::string path)
{
    sf::Font font;
    if(!font.openFromFile(path)) { return false; }
    m_fonts.emplace(name, font);
    return true;
}
