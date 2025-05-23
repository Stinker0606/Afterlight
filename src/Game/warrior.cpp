//
// Created by Kruse on 23/05/2025.
//

#include "warrior.h"
#include <iostream>
#include "config.h"

warrior::warrior() {
    hp = game::Config::warrior_hp;
    speed = game::Config::warrior_speed;
    damage = game::Config::warrior_damage;

    sprite = LoadTexture(game::Config::warrior_sprite_path);

    // If sprite failed to load

    if (sprite.id == 0) {
        std::cerr << "Failed to load Warrior sprite" << std::endl;
    }
}

warrior::~warrior() {
    UnloadTexture(sprite);
}

void warrior::UPDATE(float delta_time) {
    base_class::UPDATE(delta_time);

}

void warrior::ATTACK() {

    //Placeholder

}

// Draw the warrior at a fixed position (Default Placeholder position is X_100 Y_100)

void warrior::DRAW() const {
    DrawTexture(sprite, 100, 100, WHITE);       // Placeholder position
}
