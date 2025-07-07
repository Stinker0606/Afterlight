//
// Created by Manza on 6/22/2025.
//

#include "MeleeEnemy.h"
#include <cmath>
#include <raylib.h>

namespace enemy {

    Melee_Enemy::Melee_Enemy(std::string name, int health, float movement_speed, int damage, int value,
                             const char* sprite_path, Vector2 start_position, int width, int height,
                             float attack_range, float attack_cooldown, Collision_Manager* manager)
            : Enemy_Base_Class(name, health, movement_speed, damage, value, sprite_path, nullptr, start_position, width, height, manager,attack_cooldown),
              attack_range(attack_range),
              attack_cooldown(attack_cooldown),
              time_since_last_attack(0.0f)
    {

    }

    void Melee_Enemy::Tick(float delta_time, float target_Position_X, float target_Position_Y) {
        // First, let the enemy move towards the player using the base class logic
        this->Pathfinding(target_Position_X, target_Position_Y, delta_time);

        // Update the attack timer
        time_since_last_attack += delta_time;

        // Check conditions for an attack
        if (time_since_last_attack >= attack_cooldown) {
            float delta_vector_x = target_Position_X - this->hitbox.x;
            float delta_vector_y = target_Position_Y - this->hitbox.y;
            float distance_to_target = std::sqrt(delta_vector_x * delta_vector_x + delta_vector_y * delta_vector_y);

            if (distance_to_target <= attack_range) {
                this->Attack();
                time_since_last_attack = 0.0f;
            }
        }
    }

    void Melee_Enemy::Attack() {
        // This is where the specific attack logic would go.
        // For now, it's a placeholder.
    }
}