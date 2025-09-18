#pragma once

#include "../EnemyBaseClass.h"
#include "../core/ControllableAnimations.h"

class Object_Manager;

namespace enemy
{
    class Corpse : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { WALKING, ATTACKING, DYING };
        AnimationState anim_state_;
        Vector2 last_player_position_;
        ControllableAnimations* p_current_animation_;
        bool melee_hitbox_spawned_;

        // --- ANIMATIONS ---
        ControllableAnimations anim_walk_front_;
        ControllableAnimations anim_walk_back_;
        ControllableAnimations anim_walk_left_;
        ControllableAnimations anim_walk_right_;

        ControllableAnimations anim_attack_front_;
        ControllableAnimations anim_attack_back_;
        ControllableAnimations anim_attack_left_;
        ControllableAnimations anim_attack_right_;

        ControllableAnimations anim_death_;

    public:
        Corpse(Vector2 start_position, Object_Manager& om, bool use_fog = false);
        ~Corpse() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override;
        void Range_Attack() override;
        void Draw() override;

        void PlayHitSound() override;
        void PlayDeathSound() override;
        void PlayMoveSound() override;
    };
}