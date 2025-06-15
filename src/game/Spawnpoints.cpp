//
// Created by Kruse on 02/06/2025.
//

#include "Spawnpoints.h"
#include "../game/Collidable.h"

// Konstruktor initialisiert Position und Trefferpunkte
Spawnpoints::Spawnpoints(Vector2 position, int hits)
{
    // Annahme: Spawner hat eine feste Größe, z.B. 64x64
    this->pos = position;
    this->hitbox = { position.x, position.y, 64, 64 };
    this->spawner_Hits = hits;
}

CollisionType Spawnpoints::Get_Collision_Type() const
{
    return CollisionType::ENEMY;
}

void Spawnpoints::On_Collision(Collidable* other)
{
    // Der Spawner soll Schaden von Spieler-Projektilen oder Nahkampfangriffen nehmen
    CollisionType type = other->Get_Collision_Type();

    if (type == CollisionType::PLAYER_PROJECTILE || type == CollisionType::PLAYER_MELEE_HITBOX)
    {
        this-> spawner_Hits--;

        // Wichtig: Das Projektil/der Nahkampfangriff muss sich selbst zerstören.
        // Das passiert in dessen eigener On_Collision-Methode.
    }
}

bool Spawnpoints::Is_Destroyed() const
{
    return spawner_Hits <= 0;
}