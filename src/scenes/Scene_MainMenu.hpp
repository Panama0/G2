#include "scene base/Scene.hpp"

class Scene_MainMenu : public Scene
{
public:
    enum ActionTypes
    {
        null,
        FS,
    };
    
    Scene_MainMenu() = default;
    Scene_MainMenu(GameEngine* game)
        :Scene(game)
    { init(); }
    
    void update();
    void sDoAction(const Action& action);
    void sRender();
    void sAnimation();
    void init();
private:
    void spawnMainMenu();
    void spawnButton(const std::string& name, std::string_view tex, sf::Vector2f worldPos);
    
    sf::View m_view;
};