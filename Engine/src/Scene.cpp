#include "Scene.hpp"
#include "Buttons.hpp"

void Scene::registerAction(Buttons::Button button, int typeIndex)
{
    m_actions[button] = typeIndex;
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
