//
// Created by Kruse on 02/06/2025.
//

#include "Consumables.h"
#include <iostream>
#include "CollisionManager.h"
#include "../../cmake-build-debug/_deps/raylib-src/src/config.h"

Consumables::Consumables(Vector2 position, std::string name, Collision_Manager* manager)
{
    name = name;
    texture = LoadTexture("assets/graphics/ball.png"); // Beispielpfad
    hitbox = { position.x, position.y, (float)texture.width, (float)texture.height };

    manager_ptr = manager;
    if (manager_ptr)
    {
        manager_ptr->Regist_Object(this);
    }
}

Consumables::~Consumables()
{
    if (manager_ptr)
    {
        manager_ptr->Unregist_Object(this);
    }
    UnloadTexture(texture);
}

Collision_Type Consumables::Get_Collision_Type() const
{
    return Collision_Type::CONSUMABLE;
}


//Core Methoden
void Consumables::Tick(float delta_time)
{

}

void Consumables::On_Collision(Collidable* other)
{

}

void Consumables::Draw()
{

}
