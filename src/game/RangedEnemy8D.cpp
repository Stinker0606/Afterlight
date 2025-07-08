//
// Created by Manza on 6/22/2025.
//

#include "RangedEnemy8D.h"
#include <cmath>
#include <raylib.h>

/** WICHTIGER HINWEIS ZUR KOMPATIBILITÄT (M_PI)
*
* Zweck: Dieser Block stellt sicher, dass die mathematische Konstante M_PI (Pi)
* für unsere Winkelberechnungen (z.B. in der Determine_Attack_Direction Funktion)
* immer existiert.
*
* Problem: Je nach Betriebssystem oder Compiler ist M_PI manchmal nicht standardmäßig
* in der Mathe-Bibliothek (<cmath>) enthalten. Das würde dazu führen, dass der Code
* auf dem PC eines anderen Team-Mitglieds plötzlich nicht mehr kompiliert.
*
* Lösung: Wir definieren M_PI hier manuell, aber NUR, wenn es nicht schon
* vorhanden ist (#ifndef), um Fehler zu vermeiden.
*
* BITTE DIESEN BLOCK NICHT ENTFERNEN!
*/

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy {
    Ranged_Enemy_8_Directional::Ranged_Enemy_8_Directional(std::string name, int health, float movement_speed, int damage, int value,
                                                           const char* enemy_sprite_path, const char* proj_sprite_path,
                                                           Vector2 start_position, int width, int height,
                                                           float attack_range, float attack_cooldown, float projectile_speed)
            : Enemy_Base_Class(name, health, movement_speed, damage, value, enemy_sprite_path, proj_sprite_path, start_position, width, height,attack_cooldown),
              attack_range(attack_range),
              attack_cooldown(attack_cooldown),
              projectile_speed(projectile_speed),
              time_since_last_attack(0.0f) {}

    void Ranged_Enemy_8_Directional::Tick(float delta_time, float target_Position_X, float target_Position_Y) {
        this->Pathfinding(target_Position_X, target_Position_Y, delta_time);
        time_since_last_attack += delta_time;

        // Check attack conditions
        if (time_since_last_attack >= attack_cooldown) {
            this->Attack(target_Position_X, target_Position_Y);
        }

        for (auto& p : sp_projectiles) {
            p->Tick(delta_time);
        }
    }

    void Ranged_Enemy_8_Directional::Draw() const {

        // Draw all active projectiles
        for (const auto& p : sp_projectiles) {
            p->Draw();
        }
    }

    void Ranged_Enemy_8_Directional::Attack(float target_Position_X, float target_Position_Y) {
        float delta_vector_x = target_Position_X - this->hitbox.x;
        float delta_vector_y = target_Position_Y - this->hitbox.y;
        float distance_to_target = std::sqrt(delta_vector_x * delta_vector_x + delta_vector_y * delta_vector_y);

        if (distance_to_target <= attack_range) {
            float normalized_direction_x = delta_vector_x / distance_to_target;
            float normalized_direction_y = delta_vector_y / distance_to_target;

            Attack_Direction discrete_direction = Determine_Attack_Direction(normalized_direction_x, normalized_direction_y);
            Vector2 fire_direction = {0, 0};

            // Convert the enum direction into a normalized vector
            switch (discrete_direction) {
                case Attack_Direction::RIGHT:      fire_direction = {1.0f, 0.0f}; break;
                case Attack_Direction::LEFT:       fire_direction = {-1.0f, 0.0f}; break;
                case Attack_Direction::UP:         fire_direction = {0.0f, -1.0f}; break;
                case Attack_Direction::DOWN:       fire_direction = {0.0f, 1.0f}; break;
                case Attack_Direction::UP_RIGHT:   fire_direction = {0.707f, -0.707f}; break;
                case Attack_Direction::UP_LEFT:    fire_direction = {-0.707f, -0.707f}; break;
                case Attack_Direction::DOWN_RIGHT: fire_direction = {0.707f, 0.707f}; break;
                case Attack_Direction::DOWN_LEFT:  fire_direction = {-0.707f, 0.707f}; break;
                case Attack_Direction::NONE: return;
            }

            // Enter in *** the path of the projectile .png -> "***"
            sp_projectiles.push_back(std::make_unique<game::Enemy_Projectile>(
                    Vector2{this->hitbox.x, this->hitbox.y},
                    fire_direction, this->projectile_speed, this->enemy_Damage,
                    this->projectile_sprite_path
            ));
            time_since_last_attack = 0.0f;
        }
    }

    Attack_Direction Ranged_Enemy_8_Directional::Determine_Attack_Direction(float direction_x, float direction_y) {
        // Use atan2 to get the angle in radians, then convert to degrees.
        float angle = atan2(direction_y, direction_x) * (180.0f / M_PI);
        if (angle < 0) {
            angle += 360;
        }

        if (angle >= 337.5 || angle < 22.5) return Attack_Direction::RIGHT;
        if (angle >= 22.5 && angle < 67.5) return Attack_Direction::DOWN_RIGHT;
        if (angle >= 67.5 && angle < 112.5) return Attack_Direction::DOWN;
        if (angle >= 112.5 && angle < 157.5) return Attack_Direction::DOWN_LEFT;
        if (angle >= 157.5 && angle < 202.5) return Attack_Direction::LEFT;
        if (angle >= 202.5 && angle < 247.5) return Attack_Direction::UP_LEFT;
        if (angle >= 247.5 && angle < 292.5) return Attack_Direction::UP;
        if (angle >= 292.5 && angle < 337.5) return Attack_Direction::UP_RIGHT;

        return Attack_Direction::NONE;
    }
}