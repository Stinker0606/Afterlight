#include "Explosion.h"
#include "BreakableWall.h"
#include "../EnemyBaseClass.h"
#include "../PlayerClass.h"
#include "../../config.h.in"

Explosion::Explosion(Vector2 position)
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
    if (!other) return;

    // Bomben machen ALLEN Objekten Schaden
    if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(other)) {
        enemy->Take_Damage(game::Config::kBombDamage);
    }
    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        player->Take_Damage(game::Config::kBombDamage);
    }

    // Zerstörbare Wände reagieren selbst auf die Explosion
    if (auto breakable_wall = std::dynamic_pointer_cast<BreakableWall>(other)) {
        breakable_wall->On_Collision(shared_from_this());
    }
}

Collision_Type Explosion::Get_Collision_Type() const
{
    return Collision_Type::PLAYER_PROJECTILE; // Verhält sich wie ein Projektil, um Schaden zu verursachen
}