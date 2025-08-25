#pragma once

#include "../EnemyBaseClass.h"
#include "../../core/RepeatAnimation.h"

namespace enemy
{
    class DrownedSniper : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { IDLE, ATTACKING, DYING };
        AnimationState anim_state_;

        RepeatAnimation* p_current_animation_;

        // --- Animationen ---
        RepeatAnimation anim_idle_; // Idle wird der Standard-Zustand sein
        RepeatAnimation anim_death_;
        RepeatAnimation anim_attack_up_;
        RepeatAnimation anim_attack_down_;
        RepeatAnimation anim_attack_left_;
        RepeatAnimation anim_attack_right_;
        RepeatAnimation anim_attack_up_left_;
        RepeatAnimation anim_attack_up_right_;
        RepeatAnimation anim_attack_down_left_;
        RepeatAnimation anim_attack_down_right_;

        float attack_animation_timer_;
        Vector2 last_player_position_;

    public:
        DrownedSniper(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~DrownedSniper() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override {}; // Hat keinen Nahkampfangriff
        void Range_Attack() override;

        void Draw() override;
    };
}