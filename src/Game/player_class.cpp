//
// Created by Kruse on 23/05/2025.
//

#include "player_class.h"
#include "../config.h.in"


void player_base_class::UPDATE(float delta_time) {
    hp_timer += delta_time;

    if (hp_timer >= 1.0f) {
        hp -= static_cast<int>(game::Config::hp_drain);
        hp_timer = 0.0f; // Reset timer
    }
}

// !! not sure if it works correctly !!
