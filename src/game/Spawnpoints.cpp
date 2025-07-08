//
// Created by Kruse on 02/06/2025.
//

#include "Spawnpoints.h"
#include "../game/Collidable.h"
#include "CollisionManager.h"

Spawnpoints::Spawnpoints(Vector2 position, int hits)
{
    this->spawner_Hits = hits;
    this->hitbox = { position.x, position.y, 64, 64 }; // Annahme einer festen Größe



}

Spawnpoints::~Spawnpoints()
{
    // NEU: Abmelde-Logik

}



Collision_Type Spawnpoints::Get_Collision_Type() const
{
    return Collision_Type::ENEMY_SPAWNER; // KORRIGIERT (war vorher ENEMY)
}

bool Spawnpoints::Is_Destroyed() const { return spawner_Hits <= 0; }

void Spawnpoints::On_Collision(Collidable* other)
{
    Collision_Type type = other->Get_Collision_Type();
    if (type == Collision_Type::PLAYER_PROJECTILE || type == Collision_Type::PLAYER_MELEE_HITBOX)
    {
        this->spawner_Hits--;
    }
}

void Spawnpoints::Tick(float delta_time) {}
void Spawnpoints::Draw() {}