#include "GameEngine.hpp"
#include "scenes/Scene_Editor.hpp"

#include <memory>

int main()
{
    GameEngine g;
    g.startScene(std::make_unique<Scene_Editor>(&g, g.generateID()));
    g.run();
}
