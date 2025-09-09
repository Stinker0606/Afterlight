cmake_minimum_required(VERSION 3.11)

add_library(scenes STATIC
        Scene.h
        MenuScene.h
        MenuScene.cpp
        GameScene.h
        GameScene.cpp
        PauseScene.cpp
        PauseScene.h

        # Unsere neuen Klassen
        LevelScreen.h
        LevelScreen.cpp
        LevelScene.h
        LevelScene.cpp

        # Dein FogManager
        FogManager.h
        FogManager.cpp
        UIManager.cpp
        UIManager.h
        DeathScene.cpp
        DeathScene.h
)

target_include_directories(scenes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Die Szenen-Bibliothek muss wissen, wo die Kern-Logik ist
target_link_libraries(scenes raylib raylib_cpp core nlohmann_json::nlohmann_json)
