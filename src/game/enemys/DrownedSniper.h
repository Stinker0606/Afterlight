#pragma once

#include "../EnemyBaseClass.h"
#include "../../core/RepeatAnimation.h"

namespace enemy
{
    class DrownedSniper : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { RELOADING, ATTACKING, DYING };
        AnimationState anim_state_;

        RepeatAnimation* p_current_animation_;

        // --- Animationen ---
        RepeatAnimation anim_death_;

        RepeatAnimation anim_attack_up_;
        RepeatAnimation anim_attack_down_;
        RepeatAnimation anim_attack_left_;
        RepeatAnimation anim_attack_right_;
        RepeatAnimation anim_attack_up_left_;
        RepeatAnimation anim_attack_up_right_;
        RepeatAnimation anim_attack_down_left_;
        RepeatAnimation anim_attack_down_right_;

        RepeatAnimation anim_reload_up_;
        RepeatAnimation anim_reload_down_;
        RepeatAnimation anim_reload_left_;
        RepeatAnimation anim_reload_right_;
        RepeatAnimation anim_reload_up_left_;
        RepeatAnimation anim_reload_up_right_;
        RepeatAnimation anim_reload_down_left_;
        RepeatAnimation anim_reload_down_right_;

        // Timer für die Angriffs-Sequenz
        float attack_animation_timer_; // Steuert die Dauer der Schuss-Animation
        float reload_timer_;           // Steuert die Dauer der Nachlade-Phase (das Zielen)
        bool has_fired_;               // Stellt sicher, dass nur ein Schuss pro Angriff abgefeuert wird

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