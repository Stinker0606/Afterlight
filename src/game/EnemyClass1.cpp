//
// Created by $Will on 09.06.2025.
//

#include "EnemyClass1.h"
#include "../config.h.in"

namespace enemy 
{
    Enemy_Class_1::Enemy_Class_1(Vector2 start_position)
        : Enemy_Base_Class("EnemyClass1",game::Config::enemy_Class_1_Health, game::Config::enemy_Class_1_Movement_Speed,
              game::Config::enemy_Class_1_Damage, game::Config::enemy_Class_1_Value, start_position,
              game::Config::enemy_Class_1_Hitbox_Width, game::Config::enemy_Class_1_Hitbox_Height
          )
    {
        this->sprite = LoadTexture(game::Config::enemy_Class_1_Sprite_Path);
    }

    Enemy_Class_1::~Enemy_Class_1()
    {
        UnloadTexture(this->sprite);
    }

    void Enemy_Class_1::Attack() {}

    void Enemy_Class_1::Draw()
    {
        DrawTexture(this->sprite, this->enemy_Hitbox.x, this->enemy_Hitbox.y, WHITE);
    }

    void Enemy_Class_1::Pathfinding()
    {
        // Logik für die Wegfindung des Enemy_Class_1s
    }
}