#include "Scene.hpp"

#include <iostream>

void Scene::registerAction(sf::Keyboard::Key key, int index)
{
    m_actionMap[key] = index;
}

void Scene::registerAsset(AssetType type, std::string name, std::string path, uint32_t frames, uint32_t interval)
{
    if(type == AssetType::font)
    {
        m_assets.addFont(name, path);
    }
    else if(type == AssetType::texture)
    {
        m_assets.addTexture(name, path);
    }
    else if(type == AssetType::texture)
    {
        m_assets.addTexture(name, path);
    }
    else if(type == AssetType::animation)
    {
        assert((frames != 0 || interval != 0) && "Cant add empty animation!");
        m_assets.addAnimation(name, frames, interval);
    }
}