#pragma once

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "Animation.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

class Assets
{
public:
    bool addTexture(std::string_view name, const std::filesystem::path& fname);
    bool addAnimation(std::string_view name,
                      const std::filesystem::path& fname,
                      uint32_t frames,
                      uint32_t interval);
    bool addSound(std::string_view name, const std::filesystem::path& fname);
    bool addFont(std::string_view name, const std::filesystem::path& fname);

    bool loadTextureDir(const std::filesystem::path path);

    const sf::Texture& getTexture(std::string_view name)
    {
        return m_textures[std::string(name)];
    }
    const Animation& getAnimation(std::string_view name)
    {
        return m_animations[std::string(name)];
    }
    const sf::SoundBuffer& getSound(std::string_view name)
    {
        return m_sounds[std::string(name)];
    }
    const sf::Font& getFont(std::string_view name)
    {
        return m_fonts[std::string(name)];
    }

    const std::vector<std::string>& getTextureList() { return m_textureList; }

    void setResourceDir(const std::filesystem::path& path)
    {
        m_resourcesDir = path;
    }

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, Animation> m_animations;
    std::unordered_map<std::string, sf::SoundBuffer> m_sounds;
    std::unordered_map<std::string, sf::Font> m_fonts;

    std::vector<std::string> m_textureList;
    std::vector<std::string> m_animationList;
    std::vector<std::string> m_soundList;
    std::vector<std::string> m_fontList;

    std::filesystem::path m_resourcesDir;
};