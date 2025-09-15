#pragma once

#include "../EnemyBaseClass.h"
#include "../core/ControllableAnimations.h"

class Object_Manager;

namespace enemy
{
    class DarknessMonster : public Enemy_Base_Class
    {
    private:
        enum class AIState { CHASING_RANGED, CHASING_MELEE, ATTACKING_RANGED, ATTACKING_MELEE, DYING };
        AIState ai_state_;

        Vector2 last_player_position_;
        ControllableAnimations* p_current_animation_;

        bool has_attacked_in_state_;

        // --- NEU: Eigener Timer für den Fernkampf ---
        float ranged_attack_cooldown_timer_;

        // --- ANIMATIONS ---
        ControllableAnimations anim_death_;

        // Walk (4 Richtungen)
        ControllableAnimations anim_walk_front_;
        ControllableAnimations anim_walk_back_;
        ControllableAnimations anim_walk_left_;
        ControllableAnimations anim_walk_right_;

        // Melee (4 Richtungen)
        ControllableAnimations anim_melee_front_;
        ControllableAnimations anim_melee_back_;
        ControllableAnimations anim_melee_left_;
        ControllableAnimations anim_melee_right_;

        // Ranged (8 Richtungen)
        ControllableAnimations anim_ranged_up_;
        ControllableAnimations anim_ranged_down_;
        ControllableAnimations anim_ranged_left_;
        ControllableAnimations anim_ranged_right_;
        ControllableAnimations anim_ranged_up_left_;
        ControllableAnimations anim_ranged_up_right_;
        ControllableAnimations anim_ranged_down_left_;
        ControllableAnimations anim_ranged_down_right_;


    public:
        DarknessMonster(Vector2 start_position, Object_Manager& om);
        ~DarknessMonster() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override;
        void Range_Attack() override;
        void Draw() override;

        void PlayHitSound() override;
        void PlayDeathSound() override;
    };
}