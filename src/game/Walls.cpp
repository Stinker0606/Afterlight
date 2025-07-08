//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
#include "CollisionManager.h"

Walls::Walls(Vector2 position, Vector2 size)
{
    hitbox = { position.x, position.y, size.x, size.y };

}

Walls::~Walls()
{

}

Collision_Type Walls::Get_Collision_Type() const
{
    return Collision_Type::WALL;
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
