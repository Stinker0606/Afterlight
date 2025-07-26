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
        Insect_Monster(Vector2 start_position);
        ~Insect_Monster() override = default;

        /**
         * @brief Die Tick-Methode wird jeden Frame aufgerufen.
         * Hier wird die KI gesteuert (z.B. wann der Gegner angreift).
         * @param delta_time Die Zeit seit dem letzten Frame.
         * @param player_position Die aktuelle Position des Spielers.
         */
        void Tick(float delta_time, Vector2 player_position);

        /**
         * @brief Zeichnet das Insektenmonster.
         * Hier wird später die Animationslogik implementiert.
         */
        void Draw() override;
    };
}
