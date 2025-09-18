#include "Explosion.h"
#include "BreakableWall.h"
#include "../EnemyBaseClass.h"
#include "../PlayerClass.h"
#include "../../config.h.in"

Explosion::Explosion(Vector2 position, Object_Manager& om)
    : om_ref_(om),
      animation_(
          game::Config::kExplosionAnimationSize,
          game::Config::kExplosionAnimationPath,
          game::Config::kExplosionAnimationFrames,
          game::Config::kExplosionAnimationFramesPerLine,
          game::Config::kExplosionTimings,
          false
      )
{
    // Die Explosion ist ein 3x3 Kachel großes Feld (96x96), zentriert auf der Bombe
    hitbox = { position.x - 32.0f, position.y - 32.0f, 96.0f, 96.0f };
    lifetime = 0.2f; // Die Explosion existiert nur für einen sehr kurzen Moment
}

void Explosion::Tick(float delta_time)
{
    lifetime -= delta_time;
    if (lifetime <= 0.0f) {}

    animation_.Next_Frame();

    // Wenn die Animation fertig ist, zerstöre das Objekt.
    if (lifetime <= -6.6f) {
        this->Mark_For_Destruction();
    }
}

void Explosion::Draw()
{
    Vector2 draw_pos = {
        hitbox.x + hitbox.width / 2,
        hitbox.y + hitbox.height / 2
    };
    animation_.Draw_Current_Frame(draw_pos);
}

void Explosion::On_Collision(std::shared_ptr<Collidable> other)
{
    if (lifetime > 0.0f)
    {
        if (auto breakable_wall = std::dynamic_pointer_cast<BreakableWall>(other))
        {
            float search_radius = 80.0f; // 2,5 Kacheln
            Vector2 explosion_center = this->Get_Hitbox_Center();

            // Gehe durch ALLE Objekte
            for (const auto& obj_to_check : om_ref_.managed_objects)
            {
                if (auto wall_to_destroy = std::dynamic_pointer_cast<BreakableWall>(obj_to_check))
                {
                    if (Vector2Distance(explosion_center, wall_to_destroy->Get_Hitbox_Center()) <= search_radius)
                    {
                        wall_to_destroy->DestroyWall();
                    }
                }
            }
        }

        if (!other) return;

        // Bomben machen ALLEN Objekten Schaden
        if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(other)) {
            enemy->Take_Damage(game::Config::kBombDamage);
        }
        if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
            player->Take_Damage(game::Config::kBombDamage);
        }
    }
}

Collision_Type Explosion::Get_Collision_Type() const
{
    return (lifetime > 0.0f) ? Collision_Type::PLAYER_PROJECTILE : Collision_Type::WALL;
}