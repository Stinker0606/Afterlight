#include "MeleeHitbox.h"
#include "../EnemyBaseClass.h"
#include "../PlayerClass.h"

MeleeHitbox::MeleeHitbox(Rectangle hitbox, float lifetime, int damage, Collision_Type owner_type)
    : lifetime_(lifetime), damage_(damage), owner_type_(owner_type)
{
    this->hitbox = hitbox;
}

void MeleeHitbox::Tick(float delta_time)
{
    lifetime_ -= delta_time;
    if (lifetime_ <= 0.0f) {
        this->Mark_For_Destruction();
    }
}

Collision_Type MeleeHitbox::Get_Collision_Type() const
{
    // Die Hitbox identifiziert sich basierend darauf, wer sie erzeugt hat.
    return (owner_type_ == Collision_Type::PLAYER) ? Collision_Type::PLAYER_MELEE_HITBOX : Collision_Type::ENEMY_MELEE_HITBOX;
}

void MeleeHitbox::On_Collision(std::shared_ptr<Collidable> other)
{
    if (hit_counter_ <= 0 || std::find(already_hit_.begin(), already_hit_.end(), other) != already_hit_.end()) {
        return;
    }

    bool is_valid_target = (owner_type_ == Collision_Type::PLAYER && other->Get_Collision_Type() == Collision_Type::ENEMY) ||
                           (owner_type_ == Collision_Type::ENEMY && other->Get_Collision_Type() == Collision_Type::PLAYER);

    if (is_valid_target) {
        if (auto enemy = std::dynamic_pointer_cast<enemy::Enemy_Base_Class>(other)) {
            enemy->Take_Damage(damage_);
        } else if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
            player->Take_Damage(damage_);
        }
        hit_counter_--;
        already_hit_.push_back(other);
    }
}