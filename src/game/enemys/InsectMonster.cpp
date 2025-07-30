#include "InsectMonster.h"
#include "../../config.h.in"

namespace enemy
{
    Insect_Monster::Insect_Monster(Vector2 start_position)
        : Enemy_Base_Class(
            "Insect Monster",
            game::Config::kInsectMonsterHealth,
            game::Config::kInsectMonsterMovementSpeed,
            game::Config::kInsectMonsterDamage,
            game::Config::kInsectMonsterValue,
            game::Config::kInsectMonsterSpritePath,
            nullptr,
            start_position,
            game::Config::kInsectMonsterHitboxWidth,
            game::Config::kInsectMonsterHitboxHeight,
            game::Config::kInsectMonsterAttackCooldown
          )
    {}

    void Insect_Monster::Tick(float delta_time, Vector2 player_position)
    {
        // Rufe zuerst die Basis-Tick-Funktion auf (z.B. für Cooldowns)
        Enemy_Base_Class::Tick(delta_time);

        // Führe dann die Pathfinding-Logik aus, um dem Spieler zu folgen.
        Pathfinding(player_position.x, player_position.y, delta_time);
    }

    // NEU: Implementierung der fehlenden Tick-Funktion
    void Insect_Monster::Tick(float delta_time)
    {
        // Die allgemeine Update-Schleife ruft diese Funktion auf.
        // Da wir hier die Spielerposition nicht kennen, rufen wir unsere KI-Tick-Funktion
        // mit einer Platzhalter-Position auf. Die eigentliche Verfolgung wird von der
        // Level1Scene gesteuert, die die detailliertere Tick-Funktion aufruft.
        Tick(delta_time, Vector2{0, 0});
    }

    void Insect_Monster::Draw()
    {
        if (sprite.id > 0)
        {
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}
