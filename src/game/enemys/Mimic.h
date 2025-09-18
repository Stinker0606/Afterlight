#pragma once

#include "../EnemyBaseClass.h"
#include "../core/ControllableAnimations.h"

class Object_Manager;

namespace enemy
{
    class Mimic : public Enemy_Base_Class
    {
    private:
        enum class MimicState { TRANSFORMING, WALKING, ATTACKING, DYING };
        MimicState current_state_;
        bool mimic_walk_sound_started_ = false;

        Vector2 last_player_position_;
        ControllableAnimations* p_current_animation_;
        bool melee_hitbox_spawned_;

        // --- ANIMATIONS ---
        ControllableAnimations anim_transform_;
        ControllableAnimations anim_death_;

        ControllableAnimations anim_walk_front_;
        ControllableAnimations anim_walk_back_;
        ControllableAnimations anim_walk_left_;
        ControllableAnimations anim_walk_right_;

        ControllableAnimations anim_attack_front_;
        ControllableAnimations anim_attack_back_;
        ControllableAnimations anim_attack_left_;
        ControllableAnimations anim_attack_right_;

    public:
        Mimic(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~Mimic() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override;
        void Range_Attack() override {};
        void Draw() override;

        void Take_Damage(int damage) override;

        void PlayHitSound() override;
        void PlayDeathSound() override;
        void PlayMoveSound() override;

    };
}