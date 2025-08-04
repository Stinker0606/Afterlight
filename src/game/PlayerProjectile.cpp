#include "PlayerProjectile.h"
#include "EnemyBaseClass.h"
#include "CollisionResponse.h"
#include "raymath.h"

namespace game
{
    // Konstruktor
Player_Projectile::Player_Projectile(Vector2 start_position, Vector2 direction, int damage, Facing_Direction facing_dir)
    : position(start_position),
      is_active(true),
      damage(damage),
      lifetime(game::Config::player_Projectile_Lifetime),
      facing_direction(facing_dir)
    {
        this->velocity.x = direction.x * game::Config::player_Projectile_Speed;
        this->velocity.y = direction.y * game::Config::player_Projectile_Speed;

        // Wähle das korrekte Sprite basierend auf der Blickrichtung
        const char* sprite_path = game::Config::kPlayerProjectileSpriteRight;
        switch (facing_direction)
        {
            case UP:         sprite_path = game::Config::kPlayerProjectileSpriteUp; break;
            case DOWN:       sprite_path = game::Config::kPlayerProjectileSpriteDown; break;
            case LEFT:       sprite_path = game::Config::kPlayerProjectileSpriteLeft; break;
            case RIGHT:      sprite_path = game::Config::kPlayerProjectileSpriteRight; break;
            case UP_LEFT:    sprite_path = game::Config::kPlayerProjectileSpriteUpLeft; break;
            case UP_RIGHT:   sprite_path = game::Config::kPlayerProjectileSpriteUpRight; break;
            case DOWN_LEFT:  sprite_path = game::Config::kPlayerProjectileSpriteDownLeft; break;
            case DOWN_RIGHT: sprite_path = game::Config::kPlayerProjectileSpriteDownRight; break;
        }
        this->sprite = LoadTexture(sprite_path);

        // Berechne die Rotation des Sprites aus dem Richtungsvektor
        this->rotation = atan2f(direction.y, direction.x) * RAD2DEG;

        // Erstelle die Hitbox
        this->hitbox = {
            position.x - game::Config::player_Projectile_Hitbox_Width / 2.0f,
            position.y - game::Config::player_Projectile_Hitbox_Height / 2.0f,
            (float)game::Config::player_Projectile_Hitbox_Width,
            (float)game::Config::player_Projectile_Hitbox_Height
        };
    }

    // Destruktor
    Player_Projectile::~Player_Projectile() {
        if (sprite.id > 0) {
            UnloadTexture(sprite);
        }
    }

    // Tick
    void Player_Projectile::Tick(float delta_time)
    {
        if (!is_active) return;

        lifetime -= delta_time;
        if (lifetime <= 0.0f) {
            this->is_active = false;
            this->Mark_For_Destruction();
            return;
        }

        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;

        // Aktualisiere die Hitbox-Position
        hitbox.x = position.x - hitbox.width / 2.0f;
        hitbox.y = position.y - hitbox.height / 2.0f;
    }

    // Draw
    void Player_Projectile::Draw() {
        if (!is_active) return;
        DrawTexturePro(
            sprite,
            (Rectangle){ 0, 0, (float)sprite.width, (float)sprite.height },
            (Rectangle){ position.x, position.y, (float)sprite.width, (float)sprite.height },
            (Vector2){ (float)sprite.width / 2, (float)sprite.height / 2 },
            rotation,
            WHITE
        );
        // --- NEU: ZEICHNE DIE HITBOX IMMER ---
    // Das Projektil zeichnet jetzt seine eigene Hitbox in leuchtendem Grün.
    // So sehen wir immer, wo sie ist, unabhängig vom globalen Debug-Modus.
    DrawRectangleLinesEx(this->hitbox, 1.0f, RED);
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
            // FÄLLE, IN DENEN DAS PROJEKTIL ZERSTÖRT WIRD:
            case Collision_Type::ENEMY:
            case Collision_Type::ENEMY_SPAWNER:
            case Collision_Type::WALL:
            // Hier können später `movWall`, `breakWall` etc. einfach hinzugefügt werden.
            {
                /// Füge dem Gegner Schaden zu, falls es einer ist.
                if (other_type == Collision_Type::ENEMY) {
                    if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(other)) {
                        enemy->Take_Damage(this->damage);
                    }
                }
                // Zerstöre das Projektil.
                is_active = false;
                this->Mark_For_Destruction();
                break;
            }

            // FÄLLE, DIE IGNORIERT WERDEN:
            default:
            {
                // Tue nichts (z.B. bei Kollision mit Consumables).
                break;
            }
        }
    }
}