//
// Created by Kruse on 23/05/2025.
//

#include "EnemyBaseClass.h"
#include "CollisionManager.h"

namespace enemy
{
Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
    const char* sprite_path, Vector2 start_position, int width, int height, Collision_Manager* manager_ptr)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed), enemy_Damage(damage),
      enemy_Value(value)
    {
    hitbox = {start_position.x, start_position.y, (float)width, (float)height};
    sprite = LoadTexture(sprite_path);
    }

Enemy_Base_Class::~Enemy_Base_Class()
{
    UnloadTexture(sprite);
}

void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    enemy_Health -= damage_amount;
}

void Enemy_Base_Class::Pathfinding()
{

}

//Core Methoden
void Enemy_Base_Class::Tick(float delta_time)
{

}
void Enemy_Base_Class::On_Collision(Collidable* other)
{

}
void Enemy_Base_Class::Draw()
{

}
}