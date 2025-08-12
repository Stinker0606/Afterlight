#pragma once

#include "../EnemyBaseClass.h"

namespace enemy
{
    /**
     * @brief Repräsentiert das Insektenmonster aus Level 1.
     * Erbt die grundlegende KI und die Stats von der EnemyBaseClass
     * und implementiert seine eigene Logik für den Nahkampf (Sweep-Attacke).
     */
    class Insect_Monster : public Enemy_Base_Class
    {
    public:
        /**
         * @brief Konstruktor für das Insektenmonster.
         * @param start_position Die Position, an der der Gegner gespawnt wird.
         */
        Insect_Monster(Vector2 start_position, bool use_fog = true);
        ~Insect_Monster() override = default;

        /**
         * @brief Die Update-Methode wird jeden Frame aufgerufen.
         * Hier wird die KI gesteuert (z.B. wann der Gegner angreift).
         * @param delta_time Die Zeit seit dem letzten Frame.
         * @param player_position Die aktuelle Position des Spielers.
         */
        void Update_AI(float delta_time, Vector2 player_position) override;

        //  Wir implementieren die Angriffsfunktionen
        void Melee_Attack() override;
        void Range_Attack() override;

        /**
         * @brief Erfüllt den Vertrag der Collidable-Klasse.
         * Leitet den Aufruf an die KI-Tick-Methode weiter.
         */
        void Tick(float delta_time) override;


        /**
         * @brief Zeichnet das Insektenmonster.
         * To Do: Hier wird später die Animationslogik implementiert.
         */
        void Draw() override;
        private:
        float attack_animation_timer;
    };
}
