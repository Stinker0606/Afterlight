//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
Walls::Walls(Vector2 position, Vector2 size)
{

    this->size=size;
    this->pos=position;
    Rectangle hb(this->pos.x, this->pos.y, this->size.x, this->size.y);
    this->hitbox=hb;

}

CollisionType Walls::Get_Collision_Type() const
{
    return CollisionType::WALL;
}

//Core Methoden
void Tick(float delta_time)
{

}
void Walls::On_Collision(Collidable* other) override
{

}
void Walls::Draw() override
{

}