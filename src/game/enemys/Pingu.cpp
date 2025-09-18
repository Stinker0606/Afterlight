#include "Pingu.h"
#include "../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "raymath.h"
#include "SoundManager.h"
#include "../../config_enemies.h.in"

namespace enemy
{
    Pingu::Pingu(Vector2 start_position, Object_Manager& om, bool use_fog)
        : Enemy_Base_Class(
            "Pingu",
            game::EnemyConfig::kPinguHealth,
            game::EnemyConfig::kPinguMovementSpeed,
            game::EnemyConfig::kPinguDamage,
            game::EnemyConfig::kPinguValue,
            game::EnemyConfig::kPinguSpritePath,
            nullptr,
            start_position,
            game::EnemyConfig::kPinguHitboxWidth,
            game::EnemyConfig::kPinguHitboxHeight,
            game::EnemyConfig::kPinguAttackCooldown,
            om
          )
    {
        this->useFog = use_fog;
    }

    void Pingu::Update_AI(float delta_time, Vector2 player_position)
    {
        last_player_position_ = player_position;
        Enemy_Base_Class::Tick(delta_time);

        if (Vector2Distance(Get_Hitbox_Center(), player_position) > 0.0f) {
            Pathfinding(player_position, delta_time, 0.0f);
        } else {
            is_Moving = false;
        }

        if (Vector2Distance(Get_Hitbox_Center(), player_position) <= game::EnemyConfig::kPinguAttackRange && attack_Cooldown_Timer <= 0.0f)
        {
            Melee_Attack();
        }
    }

    void Pingu::Melee_Attack()
   {
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        // 1. Definiere die Standardmaße für einen horizontalen Sweep.
        float sweep_width = 32.0f;
        float sweep_height = 16.0f;
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        // 2. Berechne die Richtung zum Spieler.
        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 13.0f; // Wie weit vor dem Gegner die Hitbox erscheint.

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
            2.0f, // Lebensdauer
            this->enemy_Damage,
            Collision_Type::ENEMY // WICHTIG: Der Besitzer ist ein Gegner
        );

        // 5. Sound wird gespielt.
        //SoundManager::GetInstance().PlaySfx("Pingu_attack");

        // 6. Füge die Hitbox der Welt hinzu.
        om_ref_.AddObject(sweep_hitbox);
    }

    void Pingu::Draw()
    {
        // Bestimme, ob das Sprite gespiegelt werden soll
        bool flip_horizontal = (last_player_position_.x < this->Get_Hitbox_Center().x);

        Rectangle source_rect = { 0.0f, 0.0f, (float)this->sprite.width, (float)this->sprite.height };
        if (flip_horizontal) {
            source_rect.width *= -1; // Spiegelt die Textur horizontal
        }

        Rectangle dest_rect = { this->hitbox.x, this->hitbox.y, this->hitbox.width, this->hitbox.height };
        DrawTexturePro(this->sprite, source_rect, dest_rect, {0,0}, 0.0f, Fade(this->tint_color, this->visibility_alpha));
    }
}