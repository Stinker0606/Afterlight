#pragma once

#include "../EnemyBaseClass.h"
#include "../../core/RepeatAnimation.h"

class Object_Manager;

namespace enemy
{
    /**
     * @brief Repräsentiert das Insektenmonster aus Level 1.
     * Erbt die grundlegende KI und die Stats von der EnemyBaseClass
     * und implementiert seine eigene Logik für den Nahkampf (Sweep-Attacke).
     */
    class Insect_Monster : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { FLYING, ATTACKING, DYING };
        AnimationState anim_state_;
        float attack_animation_timer;
        Vector2 last_player_position_;
        RepeatAnimation* p_current_animation_;

        // --- ANIMATIONS ---

        // Fly
        RepeatAnimation anim_fly_front_;
        RepeatAnimation anim_fly_back_;
        RepeatAnimation anim_fly_left_;
        RepeatAnimation anim_fly_right_;

        // Death
        RepeatAnimation anim_death_;

        // Attack
        RepeatAnimation anim_melee_front_;
        RepeatAnimation anim_melee_back_;
        RepeatAnimation anim_melee_left_;
        RepeatAnimation anim_melee_right_;
    public:
        /**
         * @brief Konstruktor für den Enemy.
         * @param start_position Die Position, an der der Gegner gespawnt wird.
         */
        Insect_Monster(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~Insect_Monster() override = default;

        /**
         * @brief Die Update-Methode wird jeden Frame aufgerufen.
         * Hier wird die KI gesteuert (z.B. wann der Gegner angreift).
         * @param delta_time Die Zeit seit dem letzten Frame.
         * @param player_position Die aktuelle Position des Spielers.
         */
        void Update_AI(float delta_time, Vector2 player_position) override;

        // Angriffsfunktionen
        void Melee_Attack() override;
        void Range_Attack() override;

        /**
         * @brief Zeichnet das Insektenmonster.
         * To Do: Hier wird später die Animationslogik implementiert.
         */
        void Draw() override;

        // Soundfunktionen
        void PlayHitSound() override;
        void PlayDeathSound() override;
    };
}
