cmake_minimum_required(VERSION 3.20)

project(G2)
set(LANGUAGE CXX)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(Engine)
add_subdirectory(Game)

enable_testing()

