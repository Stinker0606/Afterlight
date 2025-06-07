cmake_minimum_required(VERSION 3.11)

add_library(scenes STATIC MenuScene.h MenuScene.cpp GameScene.h GameScene.cpp PauseScene.cpp PauseScene.h screen.h screen.cpp
        screen.cpp
        screen.h)

target_include_directories(scenes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(scenes raylib raylib_cpp core nlohmann_json::nlohmann_json)