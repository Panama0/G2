project(Game)

set(SRC_FILES
    src/main.cpp
    src/scenes/Scene_MainMenu.cpp
    src/scenes/Scene_Editor.cpp
    "src/scene base/GameMap.cpp"
    "src/scene base/EditorUI.cpp"
)

set(H_FILES
    src/scenes/Scene_MainMenu.hpp
    src/scenes/Scene_Editor.hpp
    "src/scene base/GameMap.hpp"
    "src/scene base/EditorUI.hpp"
    "src/scene base/EditorState.hpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${H_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_link_libraries(${PROJECT_NAME} Engine)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
