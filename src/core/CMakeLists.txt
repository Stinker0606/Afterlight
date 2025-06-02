cmake_minimum_required(VERSION 3.11)

add_library(core STATIC game.h game.cpp store.h scene.h stage.h stage.cpp renderer.h renderer.cpp texture2d.cpp texture2d.h sprite.h sprite.cpp sprite_animated.cpp sprite_animated.h actor.cpp actor.h
        DeltaTimeMachine.cpp
        DeltaTimeMachine.h
        Cam.cpp
        Cam.h
        Playerinput.cpp
        Playerinput.h
        colission_check.cpp
        enemy_base_class.cpp
        enemy_base_class.h
        objects.cpp
        objects.h
        consumables.cpp
        consumables.h
        Walls.cpp
        Walls.h
        Spawnpoints.cpp
        Spawnpoints.h)

target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(core raylib raylib_cpp nlohmann_json::nlohmann_json)
