#include "Scene.hpp"


void Scene::registerAction(sf::Keyboard::Key key, int index)
{
    m_actionMap[key] = index;
}

void Scene::registerFont(std::string_view name, const std::filesystem::path& fname)
{
    m_assets.addFont(name, fname);
}

void Scene::registerTexture(std::string_view name, const std::filesystem::path& fname)
{
    m_assets.addTexture(name, fname);
}

void Scene::registerAnimation(std::string_view name, const std::filesystem::path& fname, uint32_t frames, uint32_t interval)
{
    m_assets.addAnimation(name, fname, frames, interval);
}

void Scene::registerSound(std::string_view name, const std::filesystem::path& fname)
{
    m_assets.addSound(name, fname);
}