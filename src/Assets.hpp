#pragma once

#include <string>
#include <map>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "Animation.hpp"

class Assets
{
public:
    bool addTexture(std::string name, std::string path);
    bool addAnimation(std::string name, uint32_t frames, uint32_t interval);
    bool addSound(std::string name, std::string path);
    bool addFont(std::string name, std::string path);
    
    const sf::Texture& getTexture(std::string name);
    const Animation& getAnimation(std::string name);
    const sf::Sound& getSound(std::string name);
    const sf::Font& getFont(std::string name);
private:
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation> m_animations;
    std::map<std::string, sf::Sound> m_sounds;
    std::map<std::string, sf::Font> m_fonts;
};