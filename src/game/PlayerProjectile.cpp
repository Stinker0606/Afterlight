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

    // Erstelle die Hitbox AN DER SPEERSPITZE
    // 1. Definiere den Offset: Die Hitbox ist vor dem Sprite-Zentrum
    float hitbox_offset_distance = (this->sprite.width / 2.0f) - (game::Config::player_Projectile_Hitbox_Width / 2.0f);
    Vector2 hitbox_offset = { hitbox_offset_distance, 0.0f };

    // 2. Rotiere den Offset, damit er zur Flugrichtung passt
    Vector2 rotated_offset = Vector2Rotate(hitbox_offset, atan2f(direction.y, direction.x));

    // 3. Setze die finale Hitbox-Position
    this->hitbox = {
        position.x + rotated_offset.x,
        position.y + rotated_offset.y,
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
        float hitbox_offset_distance = (this->sprite.width / 2.0f) - (game::Config::player_Projectile_Hitbox_Width / 2.0f);
        Vector2 hitbox_offset = { hitbox_offset_distance, 0.0f };
        Vector2 rotated_offset = Vector2Rotate(hitbox_offset, atan2f(velocity.y, velocity.x));
        hitbox.x = position.x + rotated_offset.x - (hitbox.width / 2.0f);
        hitbox.y = position.y + rotated_offset.y - (hitbox.height / 2.0f);
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