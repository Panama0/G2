#include "GameEngine.hpp"
#include "scenes/Scene_Game.hpp"

int main()
{
        GameEngine g;
        g.startScene(std::make_unique<Scene_Game>(&g));
        g.run();
}
