#include "scene base/Scene.hpp"

void Scene::registerAction(sf::Keyboard::Key key, int index)
{
    m_keyboardActions[key] = index;
}

void Scene::registerAction(sf::Mouse::Button button, int index)
{
    m_mouseActions[button] = index;
}

void Scene::registerFont(std::string_view name,
                         const std::filesystem::path& fname)
{
    m_assets.addFont(name, fname);
}

void Scene::registerTexture(std::string_view name,
                            const std::filesystem::path& fname)
{
    m_assets.addTexture(name.data(), fname);
}

void Scene::registerAnimation(std::string_view name,
                              const std::filesystem::path& fname,
                              uint32_t frames,
                              uint32_t interval)
{
    m_assets.addAnimation(name, fname, frames, interval);
}

void Scene::registerSound(std::string_view name,
                          const std::filesystem::path& fname)
{
    m_assets.addSound(name, fname);
}