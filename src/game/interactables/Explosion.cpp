#include "Explosion.h"
#include "BreakableWall.h"
#include "../EnemyBaseClass.h"
#include "../PlayerClass.h"
#include "../../config.h.in"

Explosion::Explosion(Vector2 position, Object_Manager& om)
    : om_ref_(om)
{
    // Die Explosion ist ein 3x3 Kachel großes Feld (96x96), zentriert auf der Bombe
    hitbox = { position.x - 32.0f, position.y - 32.0f, 96.0f, 96.0f };
    lifetime = 0.2f; // Die Explosion existiert nur für einen sehr kurzen Moment
}

void Explosion::Tick(float delta_time)
{
    lifetime -= delta_time;
    if (lifetime <= 0.0f) {
        this->Mark_For_Destruction();
    }
}

void Explosion::Draw()
{
    // Temporäres visuelles Feedback für die Explosion
    DrawRectangleRec(hitbox, Fade(ORANGE, 0.5f));
}

void Explosion::On_Collision(std::shared_ptr<Collidable> other)
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
                    wall_to_destroy->Mark_For_Destruction();
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

Collision_Type Explosion::Get_Collision_Type() const
{
    return Collision_Type::PLAYER_PROJECTILE; // Verhält sich wie ein Projektil, um Schaden zu verursachen
}