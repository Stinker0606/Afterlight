//
// Created by Manza on 7/4/2025.
//

#include "PlayerProjectile.h"

namespace game {
    // Konstruktor
    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path)
            : position(start_position),
              is_active(true),
              damage(damage) {

        // Die endgültige Geschwindigkeit wird aus Richtung und Speed berechnet
        this->velocity.x = direction.x * projectile_speed;
        this->velocity.y = direction.y * projectile_speed;

        // Lädt die Textur und erstellt die Hitbox basierend auf der Texturgröße
        this->sprite = LoadTexture(sprite_path);
        this->hitbox = { position.x, position.y, (float)this->sprite.width, (float)this->sprite.height };
    }

    // Destruktor
    Player_Projectile::~Player_Projectile() {
        if (sprite.id > 0) {
            UnloadTexture(sprite);
        }
    }

    // Tick
    void Player_Projectile::Tick(float delta_time) {
        if (!is_active) return; // Inaktive Projektile werden nicht bewegt

        // Bewegt das Projektil
        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;

        // Aktualisiert die Hitbox-Position damit sie mitwandert
        hitbox.x = position.x;
        hitbox.y = position.y;
    }

    // Draw
    void Player_Projectile::Draw() const {
        if (!is_active) return; // Inaktive Projektile werden nicht gezeichnet
        DrawTextureV(sprite, position, WHITE);
    }



    // Get_Collision_Type: Sagt dem CollisionManager um welchen Typ es sich handelt
    Collision_Type Player_Projectile::Get_Collision_Type() const {
        return Collision_Type::PLAYER_PROJECTILE;
    }

    // On_Collision: Definiert was passiert wenn das Projektil etwas trifft
    void Player_Projectile::On_Collision(Collidable* other) {
        // Wenn es eine Wand oder einen Gegner trifft wird es inaktiv
        if (other->Get_Collision_Type() == Collision_Type::WALL || other->Get_Collision_Type() == Collision_Type::ENEMY) {
            is_active = false;
        }
    }
}