#pragma once

#include "../EnemyBaseClass.h"
#include "../core/ControllableAnimations.h"

namespace enemy
{
    class DrownedSniper : public Enemy_Base_Class
    {
    private:
        enum class AnimationState { RELOADING, ATTACKING, DYING };
        AnimationState anim_state_;

        ControllableAnimations* p_current_animation_; // <-- TYP ÄNDERN

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

        // Timer für die Angriffs-Sequenz
        float reload_timer_;           // Steuert die Dauer der Nachlade-Phase (das Zielen)
        bool has_fired_;               // Stellt sicher, dass nur ein Schuss pro Angriff abgefeuert wird

        Vector2 last_player_position_;

    public:
        /**
         * @brief Konstruktor für den Enemy.
         * @param start_position Die Position, an der der Gegner gespawnt wird.
         */
        DrownedSniper(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~DrownedSniper() override = default;

        /**
         * @brief Die Update-Methode wird jeden Frame aufgerufen.
         * Hier wird die KI gesteuert (z.B. wann der Gegner angreift).
         * @param delta_time Die Zeit seit dem letzten Frame.
         * @param player_position Die aktuelle Position des Spielers.
         */
        void Update_AI(float delta_time, Vector2 player_position) override;

        // Angriffsfunktionen
        void Melee_Attack() override {}; // Hat keinen Nahkampfangriff
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