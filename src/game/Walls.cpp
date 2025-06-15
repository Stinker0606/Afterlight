//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
#include "CollisionManager.h"

Walls::Walls(Vector2 position, Vector2 size, Collision_Manager* manager)
{
    hitbox = { position.x, position.y, size.x, size.y };
    manager_ptr = manager;

    if (manager_ptr)
    {
        manager_ptr->Regist_Object(this);
    }
}
Walls::~Walls()
{
    if (manager_ptr)
    {
        manager_ptr->Unregist_Object(this);
    }
}

CollisionType Walls::Get_Collision_Type() const
{
    return CollisionType::WALL;
}

//Core Methoden
void Tick(float delta_time)
{

}
void Walls::On_Collision(Collidable* other)
{

}
void Walls::Draw()
{

}