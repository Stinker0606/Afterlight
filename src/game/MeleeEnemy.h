//
// Created by Manza on 6/22/2025.
//

#pragma once
#include "EnemyBaseClass.h"

class Collision_Manager;

namespace enemy {
    class Melee_Enemy : public Enemy_Base_Class {
    private:
        float attack_range;
        float attack_cooldown;
        float time_since_last_attack;

        /**
         * @brief Performs the actual attack action (e.g., plays an animation and creates a damage hitbox).
         * This is an internal helper function called by Update().
         */
        void Attack();

    public:
        /**
        * @brief Constructs a new Melee Enemy object.
        * @param name Name of the enemy.
        * @param health Initial health points.
        * @param movement_speed Speed of the enemy.
        * @param damage Damage dealt per attack.
        * @param value Score value when defeated.
        * @param sprite_path File path to the sprite texture.
        * @param start_position Initial world position.
        * @param width Width of the enemy hitbox.
        * @param height Height of the enemy hitbox.
        * @param attack_range The maximum distance from the target to initiate an attack.
        * @param attack_cooldown The minimum time in seconds between attacks.
        */
        Melee_Enemy(std::string name, int health, float movement_speed, int damage, int value,
                    const char* sprite_path, Vector2 start_position, int width, int height,
                    float attack_range, float attack_cooldown, Collision_Manager* manager);

        ~Melee_Enemy() override = default;

        /**
         * @brief Updates the enemy's state, checks for attack conditions.
         * @param delta_time The time elapsed since the last frame.
         * @param target_Position_X The player's x position.
         * @param target_Position_y The player's y position.
         */
        void Tick(float delta_time, float target_Position_X, float target_Position_Y);
    };
}