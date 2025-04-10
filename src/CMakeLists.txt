project(Game)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC_FILES 
    main.cpp
    engine/EntityManager.cpp
    engine/GameEngine.cpp
    "scene base/Scene.cpp"
    "scene base/Assets.cpp"
    scenes/Scene_MainMenu.cpp
    "scene base/Animation.cpp"
    engine/Window.cpp
    "scene base/Grid.cpp"
    scenes/Scene_Editor.cpp
    "scene base/GameMap.cpp"
    "scene base/EditorUI.cpp"
    engine/Serialisation.cpp
    sff/sffParser.cpp
)

set(H_FILES 
    engine/Action.hpp
    "scene base/Animation.hpp"
    "scene base/Assets.hpp"
    "scene base/Components.hpp"
    engine/Entity.hpp
    engine/GameEngine.hpp
    "scene base/Scene.hpp"
    scenes/Scene_MainMenu.hpp
    engine/Window.hpp
    "scene base/Grid.hpp"
    scenes/Scene_Editor.hpp
    "scene base/GameMap.hpp"
    "scene base/EditorUI.hpp"
    "engine/Gui.hpp"
    "scene base/EditorState.hpp"
    engine/Serialisation.hpp
    engine/Parser.hpp
    sff/sffNode.hpp
    sff/sffParser.hpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${H_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
option(SFML_BUILD_AUDIO "Build audio" ON)

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Audio)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
