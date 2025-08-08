#include "InsectMonster.h"


#include "raymath.h"
#include "../../config_enemies.h.in"

namespace enemy
{
    Insect_Monster::Insect_Monster(Vector2 start_position, bool use_fog)
        : Enemy_Base_Class(
            "Insect Monster",
            game::EnemyConfig::kInsectMonsterHealth,
            game::EnemyConfig::kInsectMonsterMovementSpeed,
            game::EnemyConfig::kInsectMonsterDamage,
            game::EnemyConfig::kInsectMonsterValue,
            game::EnemyConfig::kInsectMonsterSpritePath,
            nullptr,
            start_position,
            game::EnemyConfig::kInsectMonsterHitboxWidth,
            game::EnemyConfig::kInsectMonsterHitboxHeight,
            game::EnemyConfig::kInsectMonsterAttackCooldown
          )
    {
        this->useFog = true;
    }

    void Insect_Monster::Update_AI(float delta_time, Vector2 player_position)
    {
        // Rufe zuerst die Basis-Tick-Funktion auf (z.B. für Cooldowns)
        Enemy_Base_Class::Tick(delta_time);

        // Führe dann die Pathfinding-Logik aus um dem Spieler zu folgen.
        Pathfinding(player_position.x, player_position.y, delta_time);

        // ANGRIFFSLOGIK:
        float distance_to_player = Vector2Distance({this->hitbox.x, this->hitbox.y}, player_position);

        // Wenn der Spieler in Reichweite ist UND der Cooldown bereit ist...
        if (distance_to_player <= game::EnemyConfig::kInsectMonsterAttackRange && this->attack_Cooldown_Timer <= 0.0f)
        {
            // ... dann führe einen Angriff aus.
            this->Melee_Attack();
        }
    }

    void Insect_Monster::Tick(float delta_time)
    {
    }

    // Implementierung der Angriffsfunktionen
    void Insect_Monster::Melee_Attack()
    {
        // Setzt den Cooldown in der Basisklasse zurück
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        // TO-DO: Zukünftige Logik
        // 1. Setze Gegner-Zustand auf "ATTACKING"
        // 2. Spiele Angriffsanimation ab
        // 3. Erzeuge eine temporäre Hitbox für den Schaden
    }

    void Insect_Monster::Range_Attack()
    {
    }

    void Insect_Monster::Draw()
    {
        // TO-DO: Hier wird später die Animations-Logik basierend
        // auf dem Gegner-Zustand (Idle, Flying, Attacking, Dying) stehen.
        if (sprite.id > 0)
        {
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}
