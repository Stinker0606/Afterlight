#include "InsectMonster.h"
#include "../config.h.in"

namespace enemy
{
    // Der Konstruktor ruft den Konstruktor der Basisklasse mit den Werten aus der Config auf.
    Insect_Monster::Insect_Monster(Vector2 start_position)
        : Enemy_Base_Class(
            "Insect Monster", // Name
            game::Config::kInsectMonsterHealth,
            game::Config::kInsectMonsterMovementSpeed,
            game::Config::kInsectMonsterDamage,
            game::Config::kInsectMonsterValue,
            game::Config::kInsectMonsterSpritePath,
            nullptr, // Kein Projektil-Sprite, da Nahkämpfer
            start_position,
            game::Config::kInsectMonsterHitboxWidth,
            game::Config::kInsectMonsterHitboxHeight,
            game::Config::kInsectMonsterAttackCooldown
          )
    {
        // Hier können später noch spezifische Initialisierungen für das Insektenmonster hin.
    }

    void Insect_Monster::Tick(float delta_time, Vector2 player_position)
    {
        // Rufe die Standard-Tick-Methode der Basisklasse auf (z.B. für Cooldowns)
        Enemy_Base_Class::Tick(delta_time);

        // Nutze die Pathfinding-Methode der Basisklasse, um dem Spieler zu folgen.
        Pathfinding(player_position.x, player_position.y, delta_time);
    }

    void Insect_Monster::Draw()
    {
        // Vorerst zeichnen wir nur ein einfaches Rechteck als Platzhalter.
        // Später kommt hier die Animationslogik hin.
        DrawRectangleRec(this->hitbox, VIOLET);
    }
}
