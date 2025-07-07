//
// Created by Manza on 6/22/2025.
//

#include "EnemyProjectile.h"

namespace game {
    // Konstruktor
    Enemy_Projectile::Enemy_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path)
            : position(start_position),
              is_active(true),
              damage(damage) {

        this->velocity.x = direction.x * projectile_speed;
        this->velocity.y = direction.y * projectile_speed;
        this->sprite = LoadTexture(sprite_path);
        this->hitbox = { position.x, position.y, (float)this->sprite.width, (float)this->sprite.height };
    }

    // Destruktor
    Enemy_Projectile::~Enemy_Projectile() {
        if (sprite.id > 0) {
            UnloadTexture(sprite);
        }
    }

    // Tick
    void Enemy_Projectile::Tick(float delta_time) {
        if (!is_active) return;
        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;
        hitbox.x = position.x;
        hitbox.y = position.y;
    }

    // Draw
    void Enemy_Projectile::Draw() const {
        if (!is_active) return;
        DrawTextureV(sprite, position, WHITE);
    }

    // Get_Hitbox: Gibt die Kollisionsbox zurück


    // Get_Collision_Type: Gibt den Typ zurück hier für Gegner
    Collision_Type Enemy_Projectile::Get_Collision_Type() const {
        return Collision_Type::ENEMY_PROJECTILE;
    }

    // On_Collision: Definiert die Kollisionsreaktion
    void Enemy_Projectile::On_Collision(Collidable* other) {
        // Wenn es eine Wand oder den Spieler trifft wird es inaktiv
        if (other->Get_Collision_Type() == Collision_Type::WALL || other->Get_Collision_Type() == Collision_Type::PLAYER) {
            is_active = false;
        }
    }
}