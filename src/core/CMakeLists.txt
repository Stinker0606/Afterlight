cmake_minimum_required(VERSION 3.11)

add_library(core STATIC
        # --- KERN-ENGINE ---
        Game.h
        Game.cpp
        Store.h
        Stage.h
        Stage.cpp
        Renderer.h
        Renderer.cpp
        Texture2d.cpp
        Texture2d.h
        Sprite.h
        Sprite.cpp
        SpriteAnimated.cpp
        SpriteAnimated.h
        Actor.cpp
        Actor.h
        DeltaTimeMachine.cpp
        DeltaTimeMachine.h
        Cam.cpp
        Cam.h
        CollisionManager.cpp
        CollisionManager.h
        Quadtree.cpp
        Quadtree.h
        Animations.cpp
        Animations.h
        RepeatAnimation.cpp
        RepeatAnimation.h
        Object_Manager.h
        Object_Manager.cpp

        # --- GAMEPLAY-LOGIK (gehört logisch zusammen) ---
        ../game/Collidable.h
        ../game/PlayerBaseClass.h
        ../game/PlayerBaseClass.cpp
        ../game/PlayerClass.h
        ../game/PlayerClass.cpp
        ../game/PlayerProjectile.h
        ../game/PlayerProjectile.cpp
        ../game/EnemyBaseClass.h
        ../game/EnemyBaseClass.cpp
        ../game/enemys/InsectMonster.h
        ../game/enemys/InsectMonster.cpp
        ../game/EnemySpawner.h
        ../game/EnemySpawner.cpp
        ../game/spawner/Level1Spawner.h
        ../game/spawner/Level1Spawner.cpp
        ../game/Walls.h
        ../game/Walls.cpp
        ../game/interactables/Consumables.h
        ../game/interactables/Consumables.cpp
        ../game/CollisionResponse.h
        ../game/CollisionResponse.cpp
        ../game/enemys/enemies_list.h
        ../game/FacingDirection.h
        ../game/interactables/PushBlock.cpp
        ../game/interactables/PushBlock.h
        ../game/interactables/HealConsumable.cpp
        ../game/interactables/HealConsumable.h
        ../game/interactables/DmgConsumable.cpp
        ../game/interactables/DmgConsumable.h
        ../game/interactables/interact_list.h
        ../game/interactables/KeyConsumable.cpp
        ../game/interactables/KeyConsumable.h
        ../game/interactables/BombConsumable.cpp
        ../game/interactables/BombConsumable.h
        ../game/interactables/Bomb.cpp
        ../game/interactables/Bomb.h
        ../game/interactables/Explosion.cpp
        ../game/interactables/Explosion.h
        ../game/interactables/BreakableWall.cpp
        ../game/interactables/BreakableWall.h
        ../game/interactables/MeleeHitbox.cpp
        ../game/interactables/MeleeHitbox.h
)

target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(core raylib raylib_cpp nlohmann_json::nlohmann_json)
