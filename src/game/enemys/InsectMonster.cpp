#include "InsectMonster.h"
#include "../../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "raymath.h"
#include "../../config_enemies.h.in"

namespace enemy
{
    Insect_Monster::Insect_Monster(Vector2 start_position, Object_Manager& om, bool use_fog)
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
            , om ),
            attack_animation_timer(0.0f)
    {
        this->useFog = true;
    }

    void Insect_Monster::Update_AI(float delta_time, Vector2 player_position)
{
    // Rufe zuerst die Basis-Tick-Funktion auf (z.B. für Cooldowns)
    Enemy_Base_Class::Tick(delta_time);

    // --- ZUSTANDS-LOGIK ---

    // Wenn eine Angriffsanimation läuft, zähle den Timer herunter.
    if (attack_animation_timer > 0.0f) {
        attack_animation_timer -= delta_time;
    }

    // BEWEGUNG: Führe die Pathfinding-Logik nur aus, wenn gerade KEINE Angriffsanimation läuft.
    if (attack_animation_timer <= 0.0f) {
        Pathfinding(player_position.x, player_position.y, delta_time);
        last_player_position_ = player_position;
    }

    // --- ANGRIFFS-LOGIK ---
    float distance_to_player = Vector2Distance({this->hitbox.x, this->hitbox.y}, player_position);

    // Wenn der Spieler in Reichweite ist UND der Cooldown bereit ist UND keine Animation läuft...
    if (distance_to_player <= game::EnemyConfig::kInsectMonsterAttackRange && this->attack_Cooldown_Timer <= 0.0f && attack_animation_timer <= 0.0f)
    {
        // ... dann starte die Angriffs-Animation.
        attack_animation_timer = 0.8f; // Setze die Dauer der Animation.

        // Führe den eigentlichen Angriff aus.
        this->Melee_Attack();
    }
}

    void Insect_Monster::Tick(float delta_time)
    {
    }

    // Implementierung der Angriffsfunktionen
    void Insect_Monster::Melee_Attack()
    {
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        // 1. Definiere die Standardmaße für einen horizontalen Sweep.
        float sweep_width = 16.0f;
        float sweep_height = 48.0f;
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        // 2. Berechne die Richtung zum Spieler.
        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 18.0f; // Wie weit vor dem Gegner die Hitbox erscheint.

        // 3. Bestimme die primäre Angriffsrichtung (horizontal vs. vertikal)
        if (fabs(direction.x) > fabs(direction.y))
        {
            // HORIZONTALER ANGRIFF (links oder rechts)
            hitbox_width = sweep_width;
            hitbox_height = sweep_height;
            if (direction.x > 0) { // Rechts
                hitbox_pos = { enemy_center.x + offset, enemy_center.y - hitbox_height / 2 };
            } else { // Links
                hitbox_pos = { enemy_center.x - offset - hitbox_width, enemy_center.y - hitbox_height / 2 };
            }
        }
        else
        {
            // VERTIKALER ANGRIFF (oben oder unten) - Breite und Höhe tauschen
            hitbox_width = sweep_height;
            hitbox_height = sweep_width;
            if (direction.y > 0) { // Unten
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y + offset };
            } else { // Oben
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y - offset - hitbox_height };
            }
        }

        // 4. Erstelle die Hitbox mit der korrekten Form und Position.
        auto sweep_hitbox = std::make_shared<MeleeHitbox>(
            Rectangle{ hitbox_pos.x, hitbox_pos.y, hitbox_width, hitbox_height },
            0.2f, // Lebensdauer
            this->enemy_Damage,
            Collision_Type::ENEMY // WICHTIG: Der Besitzer ist ein Gegner
        );

        // 5. Füge die Hitbox der Welt hinzu.
        om_ref_.AddObject(sweep_hitbox);
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
