#include "Assets.hpp"

bool Assets::addTexture(std::string_view name, const std::filesystem::path& fname)
{
    sf::Texture texture;
    if(!texture.loadFromFile(m_resourcesDir / fname)) { return false; }
    m_textures.emplace(name, texture);
    return true;
}

bool Assets::addAnimation(std::string_view name, const std::filesystem::path& fname, uint32_t frames, uint32_t interval)
{
    Animation animation {m_resourcesDir / fname, frames, interval};
    if(!animation.isValid) { return false; }
    
    m_animations.emplace(name, animation);
    
    return true;
}

bool Assets::addSound(std::string_view name, const std::filesystem::path& fname)
{
    
}

bool Assets::addFont(std::string_view name, const std::filesystem::path& fname)
{
    sf::Font font;
    if(!font.openFromFile(m_resourcesDir / fname)) { return false; }
    m_fonts.emplace(name, font);
    return true;
}
