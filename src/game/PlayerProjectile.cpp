//
// Created by Manza on 7/4/2025.
//

#include "PlayerProjectile.h"
#include "EnemyBaseClass.h" // Wichtig damit wir den Gegner-Typ kennen
#include "CollisionResponse.h" // Für die Zerstörung

namespace game
{
    // Konstruktor
    Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, int damage, const char* sprite_path)
            : position(start_position),
              is_active(true),
              damage(damage),
              lifetime(game::Config::player_Projectile_Lifetime)
    {
        // Die endgültige Geschwindigkeit wird aus Richtung und Speed berechnet
        this->velocity.x = direction.x * game::Config::player_Projectile_Speed;
        this->velocity.y = direction.y * game::Config::player_Projectile_Speed;

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


        // Reduziere die Lebensdauer
        lifetime -= delta_time;
        if (lifetime <= 0.0f)
        {
            // Wenn die Zeit abgelaufen ist, markiere das Projektil zur Zerstörung
            this->is_active = false;
            this->Mark_For_Destruction();
            return;
        }

        // Bewegt das Projektil
        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;

        // Aktualisiert die Hitbox-Position damit sie mitwandert
        hitbox.x = position.x;
        hitbox.y = position.y;
    }

    // Draw
    void Player_Projectile::Draw() {
        if (!is_active) return; // Inaktive Projektile werden nicht gezeichnet
        DrawTextureV(sprite, position, WHITE);
    }



    // Get_Collision_Type: Sagt dem CollisionManager um welchen Typ es sich handelt
    Collision_Type Player_Projectile::Get_Collision_Type() const {
        return Collision_Type::PLAYER_PROJECTILE;
    }

    // On_Collision: Definiert was passiert wenn das Projektil etwas trifft
    void Player_Projectile::On_Collision(std::shared_ptr<Collidable> other) {
        // Ignoriere die Kollision, wenn das Projektil bereits inaktiv ist.
        if (!is_active) return;

        Collision_Type other_type = other->Get_Collision_Type();

        // Wir verwenden einen Switch, um klar zu definieren, womit das Projektil kollidieren soll.
        switch (other_type)
        {
            // FALLE 1: Das Projektil trifft einen Gegner.
            case Collision_Type::ENEMY:
            {
                // Versuche, das andere Objekt sicher in einen Gegner umzuwandeln.
                if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(other))
                {
                    // Füge dem Gegner Schaden zu.
                    enemy->Take_Damage(this->damage);
                }
                // Zerstöre das Projektil nach dem Treffer.
                is_active = false;
                this->Mark_For_Destruction();
                break; // Wichtig: Beendet diesen "Fall".
            }

            // FALLE 2: Das Projektil trifft eine Wand oder einen Spawner.
            // Wir können mehrere Fälle zusammenfassen, wenn die Reaktion dieselbe ist.
            case Collision_Type::WALL:
            case Collision_Type::ENEMY_SPAWNER:
            // Zukünftige Typen wie breakWall und keyWall können hier einfach hinzugefügt werden.
            // case Collision_Type::BREAK_WALL:
            // case Collision_Type::KEY_WALL:
            {
                // Zerstöre das Projektil.
                is_active = false;
                this->Mark_For_Destruction();
                break;
            }

            // STANDARD-FALL: Wenn es etwas anderes trifft (Spieler, Consumable, etc.).
            default:
            {
                // Tue nichts. Das Projektil fliegt einfach weiter.
                break;
            }
        }
    }
}