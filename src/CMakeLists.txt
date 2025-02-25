project(Game)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC_FILES 
    main.cpp
    EntityManager.cpp
    GameEngine.cpp
    Scene.cpp
    Assets.cpp
    Scene_MainMenu.cpp
    Animation.cpp
    Window.cpp
    Grid.cpp
)

set(H_FILES 
    Action.hpp
    Animation.hpp
    Assets.hpp
    Components.hpp
    Entity.hpp
    GameEngine.hpp
    Scene.hpp
    Scene_MainMenu.hpp
    Window.hpp
    Grid.hpp
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
