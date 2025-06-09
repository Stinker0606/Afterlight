cmake_minimum_required(VERSION 3.11)

add_library(core STATIC game.h game.cpp store.h ../scenes/Scene.h stage.h stage.cpp renderer.h renderer.cpp texture2d.cpp texture2d.h sprite.h sprite.cpp sprite_animated.cpp sprite_animated.h actor.cpp actor.h
        DeltaTimeMachine.cpp
        DeltaTimeMachine.h
        Cam.cpp
        Cam.h
        ../game/EnemyBaseClass.cpp
        ../game/EnemyBaseClass.h
        ../game/Objects.cpp
        ../game/Objects.h
        ../game/Consumables.cpp
        ../game/Consumables.h
        ../game/Walls.cpp
        ../game/Walls.h
        ../game/Spawnpoints.cpp
        ../game/Spawnpoints.h)

target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(core raylib raylib_cpp nlohmann_json::nlohmann_json)
