#include "Assets.hpp"
#include <fstream>

bool Assets::addTexture(std::string_view name,
                        const std::filesystem::path& fname)
{
    sf::Texture texture;
    if(!texture.loadFromFile(currentPath() / fname))
    {
        return false;
    }
    m_textures.emplace(name, texture);
    m_textureList.emplace_back(name);
    return true;
}

bool Assets::addAnimation(std::string_view name,
                          const std::filesystem::path& fname,
                          uint32_t frames,
                          uint32_t interval)
{
    Animation animation{m_resourceSubdir / fname, frames, interval};
    if(!animation.isValid)
    {
        return false;
    }

    m_animations.emplace(name, animation);
    m_animationList.emplace_back(name);
    return true;
}

bool Assets::addSound(std::string_view name,
                      const std::filesystem::path& fname)
{
    // TODO: implement
}

bool Assets::addFont(std::string_view name, const std::filesystem::path& fname)
{
    sf::Font font;
    if(!font.openFromFile(m_resourceSubdir / fname))
    {
        return false;
    }
    m_fonts.emplace(name, font);
    m_fontList.emplace_back(name);
    return true;
}

bool Assets::loadTextureDir(const std::filesystem::path path)
{
    auto fullPath = currentPath() / path;
    if(!std::filesystem::exists(fullPath))
    {
        std::cerr << "Failed to load texture dir, path does not exist!\n";
        return false;
    }
    else if (!std::filesystem::is_directory(fullPath))
    {
        std::cerr << "Failed to load texture dir, path is not valid\n";
        return false;
    }

    for(const auto& file : std::filesystem::directory_iterator(fullPath))
    {
        sf::Texture texture;
        if(!texture.loadFromFile(file.path()))
        {
            return false;
        };
        std::string name{file.path().filename().stem().string()};
        m_textures.emplace(name, texture);
        m_textureList.emplace_back(name);
    }
    return true;
}
