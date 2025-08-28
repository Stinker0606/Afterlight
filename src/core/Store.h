#pragma once

#include <map>
#include <string>
#include <raylib.h>
#include "Stage.h"
#include "Actor.h"
#include "../game/PlayerClass.h"

namespace game::core
{
    /**
     * @brief A central place to share globally accessible objects that should be accessible from anywhere.
     */
    struct Store final
    {
        /// The Stage object is responsible for the scene change and for updating and drawing the scene contents
        inline static std::unique_ptr<game::core::Stage> stage = nullptr;

        /// To get the mouse position the mouse support must be enabled when initializing the Game object.
        /// @brief The virtual mouse position.
        inline static Vector2 mouse_Position = { };

        /// Die exakte Bildschirm-Position des Fadenkreuzes, berechnet vom UIManager.
        inline static Vector2 aiming_Position = { };

        /// Global game counter. Can be used e.g. for the calculation of animation times.
        inline static long long int ticks = 1;

        /// The actors map can contain shared pointers to actor objects that are to be used across
        /// multiple scenes (e.g. the player actor).
        inline static std::map<std::string, std::shared_ptr<game::core::Actor>> actors = { };

        // Ein globaler, permanenter Zeiger auf unseren Spieler
        inline static std::shared_ptr<PlayerClass> player = nullptr;

        // Globale Variablen für den Szenenwechsel
        inline static std::string next_scene_map = "";
        inline static std::string next_spawn_point = "";
    };
}
