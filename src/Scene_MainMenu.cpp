#include "Scene_MainMenu.hpp"

void Scene_MainMenu::update()
{
    m_entities.update();
    
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cTransform>() && entity->has<cSprite>())
        {
            auto& spr = entity->get<cSprite>().sprite;
            const auto& pos = entity->get<cTransform>().pos;
            spr.setPosition(pos);
        }
    }
    
    sRender();
}

void Scene_MainMenu::sRender()
{
    m_game->getWindow().clear(sf::Color{sf::Color::Red});
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>())
        {
            m_game->getWindow().draw(entity->get<cSprite>().sprite);
        }
    }
    m_game->getWindow().display();
}

void Scene_MainMenu::init()
{
    registerAsset(AssetType::font, "Menu Font", "../../res/DroidSans.ttf");
    registerAsset(AssetType::texture, "Button1", "../../res/button.png");
    
    spawnMainMenu();
}

void Scene_MainMenu::spawnMainMenu()
{
    spawnButton({}, "test", Action {"test", "test"});
}


void Scene_MainMenu::spawnButton(sf::FloatRect rect, std::string text, const Action& action)
{
    auto button = m_entities.addEntity("Button1");
    button->add<cSprite>(m_assets.getTexture("Button1"));
    button->add<cTransform>(sf::Vector2f{0, 20});
    button->add<cBoundingBox>();
}