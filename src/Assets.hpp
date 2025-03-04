#pragma once

#include <string>
#include <map>
#include <filesystem>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "Animation.hpp"

class Assets
{
public:
    bool addTexture(std::string_view name, const std::filesystem::path& fname);
    bool addAnimation(std::string_view name, const std::filesystem::path& fname, uint32_t frames, uint32_t interval);
    bool addSound(std::string_view name, const std::filesystem::path& fname);
    bool addFont(std::string_view name, const std::filesystem::path& fname);
    
    const sf::Texture& getTexture(std::string_view name) { return m_textures[name]; }
    const Animation& getAnimation(std::string_view  name) { return m_animations[name]; }
    const sf::SoundBuffer& getSound(std::string_view name) { return m_sounds[name]; }
    const sf::Font& getFont(std::string_view name) { return m_fonts[name]; }
    
    void setResourceDir(const std::filesystem::path& path) { m_resourcesDir = path; }
private:
    std::map<std::string_view, sf::Texture> m_textures;
    std::map<std::string_view, Animation> m_animations;
    std::map<std::string_view, sf::SoundBuffer> m_sounds;
    std::map<std::string_view, sf::Font> m_fonts;
    
    std::filesystem::path m_resourcesDir;
};