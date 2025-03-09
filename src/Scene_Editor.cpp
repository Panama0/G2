#include "Scene_Editor.hpp"

void Scene_Editor::init()
{
    //* temp
    m_gridSize = {32, 32};
    
    m_assets.setResourceDir("../../res/");
    m_globalGrid.init(m_game->getWindow().getSize(), m_gridSize);
    
    registerAction(sf::Keyboard::Key::F, static_cast<int>(ActionTypes::toggleFS));
    registerAction(sf::Keyboard::Key::G, static_cast<int>(ActionTypes::toggleGrid));
    registerAction(sf::Mouse::Button::Left, static_cast<int>(ActionTypes::place));
}

void Scene_Editor::update()
{
    sRender();
}

void Scene_Editor::sDoAction(const Action& action)
{
    switch(action.type())
    {
        case static_cast<int>(ActionTypes::toggleFS):
            if(action.status() == Action::end)
            {
                m_game->getWindow().toggleFullscreen();
            }
            break;
        
            case static_cast<int>(ActionTypes::toggleGrid):
            if(action.status() == Action::end)
            {
                if(m_gridVisible)
                {
                    m_gridVisible = false;
                }
                else
                {
                    m_gridVisible = true;
                }
            }
            break;
        
        case static_cast<int>(ActionTypes::place):
            if(action.status() == Action::start)
            {
                std::cout << action.position().x << ',' << action.position().y << '\n';
            }
    }
}

void Scene_Editor::sRender()
{
    auto& window = m_game->getWindow();
    window.beginDraw();
    if(m_gridVisible)
    {
        sf::Sprite grid {m_globalGrid.getTexture()};
        window.draw(grid);
    }
    window.render();
}