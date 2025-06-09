//
// Created by Kruse on 23/05/2025.
//

#include "EnemyBaseClass.h"

namespace enemy
{
    Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_Speed, int damage,
    int value, Vector2 start_position, int width, int height)
        : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_Speed), enemy_Damage(damage),
        enemy_Value(value)
    {
        this->enemy_Hitbox.x = start_position.x;
        this->enemy_Hitbox.y = start_position.y;
        this->enemy_Hitbox.width = width;
        this->enemy_Hitbox.height = height;
    }

    void Enemy_Base_Class::Take_Damage(int damage_amount)
    {
        this->enemy_Health -= damage_amount;

        // Muss noch weiter ausgearbeitet werden. Bedingung, sobald Leben auf 0 fällt stirbt der Gegner und gibt
        // vermutlich "valu" zurück
    }
}