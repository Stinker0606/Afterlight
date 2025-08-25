//
// Created by Kruse on 02/06/2025.
//

#include "Consumables.h"
#include <iostream>

#include "AssetManager.h"
#include "../core/CollisionManager.h"
#include "../../config.h.in"

Consumables::Consumables(Vector2 position, std::string name)
{
    name = name;
    texture = AssetManager::GetInstance().Load("assets/pngs/ball.png"); // Beispielpfad
    hitbox = { position.x, position.y, (float)texture.width, (float)texture.height };

}

Consumables::~Consumables()
{
    //UnloadTexture(texture);
}

Collision_Type Consumables::Get_Collision_Type() const
{
    return Collision_Type::CONSUMABLE;
}


//Core Methoden
void Consumables::Tick(float delta_time)
{

}

void Consumables::On_Collision(std::shared_ptr<Collidable> other)
{

}

void Consumables::Draw()
{

}



