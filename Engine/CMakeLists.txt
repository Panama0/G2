project(Engine)

include(FetchContent)

FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG master
)

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
)

FetchContent_Declare(
    ECS
    GIT_REPOSITORY https://github.com/Panama0/S-ECS
)

# Build

# SFML
option(SFML_BUILD_AUDIO "Build audio" ON)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(SFML)

# Dear ImGui
FetchContent_MakeAvailable(ImGui)

# ImGui-SFML
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)

# ECS
FetchContent_MakeAvailable(ECS)

set(SRC_FILES
    src/GameEngine.cpp
    src/SceneManager.cpp
    src/Window.cpp
    src/Scene.cpp
    src/Animation.cpp
    src/Assets.cpp
    src/Grid.cpp
    src/sff/sffParser.cpp
    src/sff/sffFileManipulator.cpp
    src/sff/sffSerialiser.cpp
)

set(H_FILES
    src/Action.hpp
    src/GameEngine.hpp
    src/SceneManager.hpp
    src/Window.hpp
    src/Gui.hpp
    src/Scene.hpp
    src/Action.hpp
    src/Grid.hpp
    src/Animation.hpp
    src/sff/sffNode.hpp
    src/sff/sffParser.hpp
    src/sff/sffFileManipulator.hpp
    src/sff/sffSerialiser.hpp
    src/sff/sffNodeData.hpp
)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${H_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC src/)

option(SFML_BUILD_AUDIO "Build audio" ON)

target_link_libraries(${PROJECT_NAME} PUBLIC SFML::Graphics SFML::Audio)
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(${PROJECT_NAME} PUBLIC ECS)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
