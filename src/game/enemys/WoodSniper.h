#pragma once

#include "../EnemyBaseClass.h"
#include "../core/ControllableAnimations.h"

namespace enemy
{
    class WoodSniper : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { RELOADING, ATTACKING, DYING };
        AnimationState anim_state_;

        ControllableAnimations* p_current_animation_;

        // --- Animationen ---
        ControllableAnimations anim_death_;

        ControllableAnimations anim_attack_up_;
        ControllableAnimations anim_attack_down_;
        ControllableAnimations anim_attack_left_;
        ControllableAnimations anim_attack_right_;
        ControllableAnimations anim_attack_up_left_;
        ControllableAnimations anim_attack_up_right_;
        ControllableAnimations anim_attack_down_left_;
        ControllableAnimations anim_attack_down_right_;

        ControllableAnimations anim_reload_up_;
        ControllableAnimations anim_reload_down_;
        ControllableAnimations anim_reload_left_;
        ControllableAnimations anim_reload_right_;
        ControllableAnimations anim_reload_up_left_;
        ControllableAnimations anim_reload_up_right_;
        ControllableAnimations anim_reload_down_left_;
        ControllableAnimations anim_reload_down_right_;

        bool has_fired_;
        Vector2 last_player_position_;

    public:
        WoodSniper(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~WoodSniper() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override {};
        void Range_Attack() override;
        void Draw() override;

        void PlayHitSound() override;
        void PlayDeathSound() override;
    };
}