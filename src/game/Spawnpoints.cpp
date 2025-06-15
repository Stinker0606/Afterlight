//
// Created by Kruse on 02/06/2025.
//

#include "Spawnpoints.h"
#include "../game/Collidable.h"
#include "CollisionManager.h"

Spawnpoints::Spawnpoints(Vector2 position, int hits, Collision_Manager* manager)
{
    this->spawner_Hits = hits;
    this->hitbox = { position.x, position.y, 64, 64 }; // Annahme einer festen Größe

    this->manager_ptr = manager;
    if (this->manager_ptr)
    {
        this->manager_ptr->Regist_Object(this);
    }
}

Spawnpoints::~Spawnpoints()
{
    // NEU: Abmelde-Logik
    if (this->manager_ptr)
    {
        this->manager_ptr->Unregist_Object(this);
    }
}

Rectangle Spawnpoints::Get_Hitbox() const { return this->hitbox; }

CollisionType Spawnpoints::Get_Collision_Type() const
{
    return CollisionType::ENEMY_SPAWNER; // KORRIGIERT (war vorher ENEMY)
}

bool Spawnpoints::Is_Destroyed() const { return spawner_Hits <= 0; }

void Spawnpoints::On_Collision(Collidable* other)
{
    CollisionType type = other->Get_Collision_Type();
    if (type == CollisionType::PLAYER_PROJECTILE || type == CollisionType::PLAYER_MELEE_HITBOX)
    {
        this->spawner_Hits--;
    }
}

void Spawnpoints::Tick(float delta_time) {}
void Spawnpoints::Draw() {}