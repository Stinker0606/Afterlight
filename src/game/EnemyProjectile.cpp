//
// Created by Manza on 6/22/2025.
//

#include "EnemyProjectile.h"

#include "AssetManager.h"
#include "raymath.h"
#include "PlayerClass.h"
#include "../config_enemies.h.in"
#include "../config.h.in"

namespace game {
    Enemy_Projectile::Enemy_Projectile(Vector2 start_position, Vector2 direction, float projectile_speed, int damage, const char* sprite_path)
            : position(start_position),
              is_active(true),
              damage(damage),
              lifetime(game::EnemyConfig::kDrownedSniperAttackRange / projectile_speed)
    {
        this->velocity.x = direction.x * projectile_speed;
        this->velocity.y = direction.y * projectile_speed;
        this->sprite = AssetManager::GetInstance().Load(sprite_path);
        this->rotation = atan2f(direction.y, direction.x) * RAD2DEG;

        // Initialisiere die Hitbox mit der korrekten Größe aus der Config
        // und zentriere sie von Anfang an.
        this->hitbox = {
            position.x - game::EnemyConfig::kDrownedSniperProjectileHitboxWidth / 2.0f,
            position.y - game::EnemyConfig::kDrownedSniperProjectileHitboxHeight / 2.0f,
            (float)game::EnemyConfig::kDrownedSniperProjectileHitboxWidth,
            (float)game::EnemyConfig::kDrownedSniperProjectileHitboxHeight
        };
    }

    Enemy_Projectile::~Enemy_Projectile() {
       /* if (sprite.id > 0) {
            UnloadTexture(sprite);
        }
    */ }

    void Enemy_Projectile::Tick(float delta_time) {
        if (!is_active) return;

        lifetime -= delta_time;
        if (lifetime <= 0.0f) {
            is_active = false;
            this->Mark_For_Destruction();
            return;
        }

        position.x += velocity.x * delta_time;
        position.y += velocity.y * delta_time;

        hitbox.x = position.x - hitbox.width / 2.0f;
        hitbox.y = position.y - hitbox.height / 2.0f;
    }

    void Enemy_Projectile::Draw()  {
        if (!is_active) return;
        DrawTexturePro(
           sprite,
           (Rectangle){ 0, 0, (float)sprite.width, (float)sprite.height },
           (Rectangle){ position.x, position.y, (float)sprite.width, (float)sprite.height },
           (Vector2){ (float)sprite.width / 2, (float)sprite.height / 2 },
           rotation,
           Fade(WHITE, visibility_alpha)
       );
    if (game::Config::kDebugShowHitboxes)
        {
            // --- DEBUG: ZEICHNE DIE HITBOX  ---
            DrawRectangleLinesEx(this->hitbox, 1.0f, RED);
        }
    }

    Collision_Type Enemy_Projectile::Get_Collision_Type() const {
        return Collision_Type::ENEMY_PROJECTILE;
    }

    void Enemy_Projectile::On_Collision(std::shared_ptr<Collidable> other) {
        if (!is_active) return;

        Collision_Type other_type = other->Get_Collision_Type();

        switch (other_type)
        {
            // FÄLLE, IN DENEN DAS PROJEKTIL ZERSTÖRT WIRD:
            case Collision_Type::PLAYER:
            case Collision_Type::WALL:
            {
                // Füge dem Spieler Schaden zu, falls es der Spieler ist.
                if (other_type == Collision_Type::PLAYER) {
                    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
                        player->Take_Damage(this->damage);
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
                // Tue nichts.
                break;
            }
        }
    }
}