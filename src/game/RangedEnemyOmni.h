//
// Created by Manza on 6/22/2025.
//

#pragma once

#include "EnemyBaseClass.h"
#include "EnemyProjectile.h"
#include "../Config.h.in"
#include <vector>
#include <memory>

class Collision_Manager;

namespace enemy {
    /**
     * @brief A ranged enemy that can fire precisely in any direction.
     */
    class Ranged_Enemy_Omni : public Enemy_Base_Class {
    private:
        float attack_range;
        float attack_cooldown;
        float projectile_speed;
        float time_since_last_attack;
        std::vector<std::unique_ptr<game::Enemy_Projectile>> sp_projectiles;

        /**
         * @brief Creates and fires a projectile towards the target.
         * @param target_Position_X The player's x position.
         * @param target_Position_Y The player's y position.
         */
        void Attack(float target_Position_X, float target_Position_Y);

    public:
        /**
        * @brief Constructs a new Omnidirectional Ranged Enemy.
        * @param name Name of the enemy.
        * @param health Initial health points.
        * @param movement_speed Speed of the enemy.
        * @param damage Damage dealt per attack.
        * @param value Score value when defeated.
        * @param enemy_sprite_path File path to the Enemy sprite texture.
        * @param proj_sprite_path File path to the Enemy Projectile sprite texture.
        * @param start_position Initial world position.
        * @param width Width of the enemy hitbox.
        * @param height Height of the enemy hitbox.
        * @param attack_range Maximum distance to attack.
        * @param attack_cooldown Minimum time between attacks.
        * @param projectile_speed The speed of fired projectiles.
        */
        Ranged_Enemy_Omni(std::string name, int health, float movement_speed, int damage, int value,
                          const char* enemy_sprite_path, const char* proj_sprite_path,
                          Vector2 start_position, int width, int height,
                          float attack_range, float attack_cooldown, float projectile_speed, Collision_Manager* manager);

        ~Ranged_Enemy_Omni() override = default;

        /**
         * @brief Updates the enemy and its projectiles.
         */
        void Tick(float delta_time, float target_Position_X, float target_Position_Y);

        /**
         * @brief Draws the enemy and its projectiles.
         */
        void Draw() const;
    };
}