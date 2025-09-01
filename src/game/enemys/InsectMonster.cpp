#include "InsectMonster.h"
#include "../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "raymath.h"
#include "../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raylib.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
            anim_fly_front_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterFlyFrontPath, game::EnemyConfig::kInsectMonsterFlyFrontFrames, game::EnemyConfig::kInsectMonsterFlyFrontFramesPerLine),
            anim_fly_back_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterFlyBackPath, game::EnemyConfig::kInsectMonsterFlyBackFrames, game::EnemyConfig::kInsectMonsterFlyBackFramesPerLine),
            anim_fly_left_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterFlyLeftPath, game::EnemyConfig::kInsectMonsterFlyLeftFrames, game::EnemyConfig::kInsectMonsterFlyLeftFramesPerLine),
            anim_fly_right_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterFlyRightPath, game::EnemyConfig::kInsectMonsterFlyRightFrames, game::EnemyConfig::kInsectMonsterFlyRightFramesPerLine),
            anim_death_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterDeathPath, game::EnemyConfig::kInsectMonsterDeathFrames, game::EnemyConfig::kInsectMonsterDeathFramesPerLine),
            anim_melee_front_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterMeleeFrontPath, game::EnemyConfig::kInsectMonsterMeleeFrontFrames, game::EnemyConfig::kInsectMonsterMeleeFrontFramesPerLine),
            anim_melee_back_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterMeleeBackPath, game::EnemyConfig::kInsectMonsterMeleeBackFrames, game::EnemyConfig::kInsectMonsterMeleeBackFramesPerLine),
            anim_melee_left_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterMeleeLeftPath, game::EnemyConfig::kInsectMonsterMeleeLeftFrames, game::EnemyConfig::kInsectMonsterMeleeLeftFramesPerLine),
            anim_melee_right_(game::EnemyConfig::kInsectMonsterAnimSize, game::EnemyConfig::kInsectMonsterMeleeRightPath, game::EnemyConfig::kInsectMonsterMeleeRightFrames, game::EnemyConfig::kInsectMonsterMeleeRightFramesPerLine),
            attack_animation_timer(0.0f)
    {
        this->useFog = true;
        this->anim_state_ = AnimationState::FLYING; // Startzustand
        this->p_current_animation_ = &anim_fly_front_; // Standard-Animation
    }

    void Insect_Monster::Update_AI(float delta_time, Vector2 player_position)
    {
        // Wenn der Gegner tot ist, mache nichts mehr.
        if (anim_state_ == AnimationState::DYING) return;

        // Prüfe, ob der Gegner sterben sollte.
        if (this->enemy_Health <= 0) {
            anim_state_ = AnimationState::DYING;
            this->attack_animation_timer = 9999.0f; // Verhindere weitere Aktionen
            return;
        }

        Enemy_Base_Class::Tick(delta_time);

        if (attack_animation_timer > 0.0f) {
            attack_animation_timer -= delta_time;
            if (attack_animation_timer <= 0.0f) {
                anim_state_ = AnimationState::FLYING; // Zurück zum Fliegen nach dem Angriff
            }
        } else {
            Pathfinding(player_position.x, player_position.y, delta_time);
            last_player_position_ = player_position;
        }

        Vector2 enemy_center = this->Get_Hitbox_Center();
        float distance_to_player = Vector2Distance(enemy_center, player_position);

        if (distance_to_player <= game::EnemyConfig::kInsectMonsterAttackRange && this->attack_Cooldown_Timer <= 0.0f && attack_animation_timer <= 0.0f)
        {
            anim_state_ = AnimationState::ATTACKING; // Setze den Angriffszustand
            attack_animation_timer = 0.8f;
            this->Melee_Attack();
        }
    }

    // Implementierung der Angriffsfunktionen
    void Insect_Monster::Melee_Attack()
    {
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        // 1. Definiere die Standardmaße für einen horizontalen Sweep.
        float sweep_width = 32.0f;
        float sweep_height = 48.0f;
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        // 2. Berechne die Richtung zum Spieler.
        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 12.0f; // Wie weit vor dem Gegner die Hitbox erscheint.

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

        // 5. Sound wird gespielt.
        SoundManager::GetInstance().PlaySfx("enemy_insect_attack");

        // 6. Füge die Hitbox der Welt hinzu.
        om_ref_.AddObject(sweep_hitbox);
    }

    // hat kein Range Attack
    void Insect_Monster::Range_Attack()
    {
    }

    void Insect_Monster::PlayHitSound() {
        SoundManager::GetInstance().PlaySfx("enemy_insect_hit");
    }

    void Insect_Monster::PlayDeathSound() {
        SoundManager::GetInstance().PlaySfx("enemy_insect_death");
    }

    void Insect_Monster::Draw()
    {
        if (game::EnemyConfig::kUseEnemyAnimations)
        {
            Vector2 direction = Vector2Normalize({ last_player_position_.x - this->Get_Hitbox_Center().x, last_player_position_.y - this->Get_Hitbox_Center().y });
            float angle = atan2(direction.y, direction.x) * (180.0f / PI);
            if (angle < 0) angle += 360;

            Facing_Direction facing_dir = Facing_Direction::DOWN;
            if (angle >= 45 && angle < 135) facing_dir = Facing_Direction::DOWN;
            else if (angle >= 135 && angle < 225) facing_dir = Facing_Direction::LEFT;
            else if (angle >= 225 && angle < 315) facing_dir = Facing_Direction::UP;
            else facing_dir = Facing_Direction::RIGHT;

            switch (anim_state_)
            {
                case AnimationState::FLYING:
                    if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_fly_back_;
                    else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_fly_front_;
                    else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_fly_left_;
                    else p_current_animation_ = &anim_fly_right_;
                break;
                case AnimationState::ATTACKING:
                    if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_melee_back_;
                    else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_melee_front_;
                    else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_melee_left_;
                    else p_current_animation_ = &anim_melee_right_;
                break;
                case AnimationState::DYING:
                    p_current_animation_ = &anim_death_;
                break;
            }

            if (p_current_animation_)
            {
                Vector2 draw_pos = {
                    this->hitbox.x - game::EnemyConfig::kInsectMonster_visual_offset.x,
                    this->hitbox.y - game::EnemyConfig::kInsectMonster_visual_offset.y
                };
                Color tint = { 255, 255, 255, (unsigned char)(this->visibility_alpha * 255.0f) };
                p_current_animation_->Draw_Current_Frame(draw_pos, tint);

                if (this->is_animation_active_) {
                    // Spiele die Animation nur ab, wenn der Schalter an ist.
                    p_current_animation_->Next_Frame();
                } else
                {
                    // Ansonsten setze sie auf den Startframe zurück.
                    p_current_animation_->First_Frame();
                }
            }
        }
        else
        {
            // --- LOGIK FÜR PLATZHALTER ---
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}