#include "DarknessMonster.h"
#include "../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "../EnemyProjectile.h"
#include "raymath.h"
#include "../../config_enemies.h.in"
#include "SoundManager.h"
#include "../FacingDirection.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy
{
    DarknessMonster::DarknessMonster(Vector2 start_position, Object_Manager& om, bool use_fog)
        : Enemy_Base_Class(
            "Darkness Monster",
            game::EnemyConfig::kDarknessMonsterHealth,
            game::EnemyConfig::kDarknessMonsterMovementSpeed,
            game::EnemyConfig::kDarknessMonsterDamage,
            game::EnemyConfig::kDarknessMonsterValue,
            nullptr, nullptr, start_position,
            game::EnemyConfig::kDarknessMonsterHitboxWidth,
            game::EnemyConfig::kDarknessMonsterHitboxHeight,
            0.0f, om
          ),
          anim_death_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterDeathPath, game::EnemyConfig::kDarknessMonsterDeathFrames, game::EnemyConfig::kDarknessMonsterDeathFramesPerLine, game::EnemyConfig::kDarknessMonsterDeathTimings, false),
          // Walk
          anim_walk_front_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterWalkFrontPath, game::EnemyConfig::kDarknessMonsterWalkFrontFrames, game::EnemyConfig::kDarknessMonsterWalkFrontFramesPerLine, game::EnemyConfig::kDarknessMonsterWalkTimings, true),
          anim_walk_back_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterWalkBackPath, game::EnemyConfig::kDarknessMonsterWalkBackFrames, game::EnemyConfig::kDarknessMonsterWalkBackFramesPerLine, game::EnemyConfig::kDarknessMonsterWalkTimings, true),
          anim_walk_left_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterWalkLeftPath, game::EnemyConfig::kDarknessMonsterWalkLeftFrames, game::EnemyConfig::kDarknessMonsterWalkLeftFramesPerLine, game::EnemyConfig::kDarknessMonsterWalkTimings, true),
          anim_walk_right_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterWalkRightPath, game::EnemyConfig::kDarknessMonsterWalkRightFrames, game::EnemyConfig::kDarknessMonsterWalkRightFramesPerLine, game::EnemyConfig::kDarknessMonsterWalkTimings, true),
          // Melee
          anim_melee_front_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterMeleeFrontPath, game::EnemyConfig::kDarknessMonsterMeleeFrontFrames, game::EnemyConfig::kDarknessMonsterMeleeFrontFramesPerLine, game::EnemyConfig::kDarknessMonsterMeleeTimings, false),
          anim_melee_back_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterMeleeBackPath, game::EnemyConfig::kDarknessMonsterMeleeBackFrames, game::EnemyConfig::kDarknessMonsterMeleeBackFramesPerLine, game::EnemyConfig::kDarknessMonsterMeleeTimings, false),
          anim_melee_left_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterMeleeLeftPath, game::EnemyConfig::kDarknessMonsterMeleeLeftFrames, game::EnemyConfig::kDarknessMonsterMeleeLeftFramesPerLine, game::EnemyConfig::kDarknessMonsterMeleeTimings, false),
          anim_melee_right_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterMeleeRightPath, game::EnemyConfig::kDarknessMonsterMeleeRightFrames, game::EnemyConfig::kDarknessMonsterMeleeRightFramesPerLine, game::EnemyConfig::kDarknessMonsterMeleeTimings, false),
          // Ranged
          anim_ranged_up_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedUpPath, game::EnemyConfig::kDarknessMonsterRangedUpFrames, game::EnemyConfig::kDarknessMonsterRangedUpFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_down_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedDownPath, game::EnemyConfig::kDarknessMonsterRangedDownFrames, game::EnemyConfig::kDarknessMonsterRangedDownFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_left_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedLeftPath, game::EnemyConfig::kDarknessMonsterRangedLeftFrames, game::EnemyConfig::kDarknessMonsterRangedLeftFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_right_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedRightPath, game::EnemyConfig::kDarknessMonsterRangedRightFrames, game::EnemyConfig::kDarknessMonsterRangedRightFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_up_left_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedUpLeftPath, game::EnemyConfig::kDarknessMonsterRangedUpLeftFrames, game::EnemyConfig::kDarknessMonsterRangedUpLeftFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_up_right_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedUpRightPath, game::EnemyConfig::kDarknessMonsterRangedUpRightFrames, game::EnemyConfig::kDarknessMonsterRangedUpRightFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_down_left_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedDownLeftPath, game::EnemyConfig::kDarknessMonsterRangedDownLeftFrames, game::EnemyConfig::kDarknessMonsterRangedDownLeftFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false),
          anim_ranged_down_right_(game::EnemyConfig::kDarknessMonsterAnimSize, game::EnemyConfig::kDarknessMonsterRangedDownRightPath, game::EnemyConfig::kDarknessMonsterRangedDownRightFrames, game::EnemyConfig::kDarknessMonsterRangedDownRightFramesPerLine, game::EnemyConfig::kDarknessMonsterRangedTimings, false)
    {
        this->useFog = true;
        this->ai_state_ = AIState::CHASING_RANGED;
        this->p_current_animation_ = &anim_walk_front_;
        this->has_attacked_in_state_ = false;
        this->attack_Cooldown_Timer = 0.0f; // Für Nahkampf
        this->ranged_attack_cooldown_timer_ = 0.0f; // Für Fernkampf
    }

    void DarknessMonster::Update_AI(float delta_time, Vector2 player_position)
    {
        last_player_position_ = player_position;

        if (ai_state_ != AIState::DYING && this->enemy_Health <= 0) {
            ai_state_ = AIState::DYING;
            anim_death_.Reset();
        }
        if (ai_state_ == AIState::DYING) {
            if (anim_death_.Is_Finished()) this->Mark_For_Destruction();
            return;
        }

        // Beide Timer aktualisieren
        Enemy_Base_Class::Tick(delta_time); // Aktualisiert attack_Cooldown_Timer für Nahkampf
        if (ranged_attack_cooldown_timer_ > 0) ranged_attack_cooldown_timer_ -= delta_time;

        float distance_to_player = Vector2Distance(Get_Hitbox_Center(), player_position);

        switch (ai_state_)
        {
            case AIState::CHASING_RANGED:
                this->enemy_Movement_Speed = game::EnemyConfig::kDarknessMonsterMovementSpeed;
                Pathfinding(player_position, delta_time, 18);

                if (distance_to_player <= game::EnemyConfig::kDarknessMonsterAggroRadius) {
                    ai_state_ = AIState::CHASING_MELEE;
                } else if (distance_to_player <= game::EnemyConfig::kDarknessMonsterRangedRange && ranged_attack_cooldown_timer_ <= 0) {
                    ai_state_ = AIState::ATTACKING_RANGED;
                    has_attacked_in_state_ = false;
                }
                break;

            case AIState::CHASING_MELEE:
                this->enemy_Movement_Speed = game::EnemyConfig::kDarknessMonsterAggroSpeed;
                Pathfinding(player_position, delta_time, 36);

                if (distance_to_player > game::EnemyConfig::kDarknessMonsterAggroRadius) {
                    ai_state_ = AIState::CHASING_RANGED;
                } else if (distance_to_player <= game::EnemyConfig::kDarknessMonsterMeleeRange && attack_Cooldown_Timer <= 0) {
                    ai_state_ = AIState::ATTACKING_MELEE;
                    has_attacked_in_state_ = false;
                }
                break;

            case AIState::ATTACKING_RANGED:
                // In diesem Zustand wird die Bewegung und Zustandsänderung blockiert, bis die Animation fertig ist.
                    if (p_current_animation_->Get_Current_Frame() == 6 && !has_attacked_in_state_) {
                        Range_Attack();
                        has_attacked_in_state_ = true;
                    }
            if (p_current_animation_->Is_Finished()) {
                ranged_attack_cooldown_timer_ = game::EnemyConfig::kDarknessMonsterRangedCooldown;
                ai_state_ = AIState::CHASING_RANGED; // Gehe erst nach der Animation zurück
            }
            break;


            case AIState::ATTACKING_MELEE:
                if (p_current_animation_->Get_Current_Frame() == 5 && !has_attacked_in_state_) {
                    Melee_Attack();
                    has_attacked_in_state_ = true;
                }
            // Der Cooldown wird jetzt hier gesetzt und der Zustand korrekt zurückgesetzt.
            if (p_current_animation_->Is_Finished()) {
                attack_Cooldown_Timer = game::EnemyConfig::kDarknessMonsterMeleeCooldown;
                ai_state_ = AIState::CHASING_MELEE;
                // Setze die Animationen zurück, um für den nächsten Angriff bereit zu sein.
                anim_melee_front_.Reset();
                anim_melee_back_.Reset();
                anim_melee_left_.Reset();
                anim_melee_right_.Reset();
            }
            break;
        }
    }

    void DarknessMonster::Melee_Attack()
    {
        // Die Logik ist jetzt fast identisch mit der des Corpse-Gegners,
        // um eine gerichtete Hitbox zu erzeugen.
        float sweep_width = 24.0f; // Breite des Angriffs
        float sweep_height = 50.0f; // Länge des Angriffs
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 13.0f; // Abstand der Hitbox vom Gegner

        if (fabs(direction.x) > fabs(direction.y)) // Horizontaler Angriff
        {
            hitbox_width = sweep_height;
            hitbox_height = sweep_width;
            if (direction.x > 0) {
                hitbox_pos = { enemy_center.x + offset, enemy_center.y - hitbox_height / 2 };
            } else {
                hitbox_pos = { enemy_center.x - offset - hitbox_width, enemy_center.y - hitbox_height / 2 };
            }
        }
        else // Vertikaler Angriff
        {
            hitbox_width = sweep_width;
            hitbox_height = sweep_height;
            if (direction.y > 0) {
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y + offset };
            } else {
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y - offset - hitbox_height };
            }
        }

        auto sweep_hitbox = std::make_shared<MeleeHitbox>(
            Rectangle{ hitbox_pos.x, hitbox_pos.y, hitbox_width, hitbox_height },
            0.3f, this->enemy_Damage, Collision_Type::ENEMY
        );
        om_ref_.AddObject(sweep_hitbox);
    }

    void DarknessMonster::Range_Attack()
    {
        Vector2 fire_direction = Vector2Normalize({ last_player_position_.x - Get_Hitbox_Center().x, last_player_position_.y - Get_Hitbox_Center().y });
        auto projectile = std::make_shared<game::Enemy_Projectile>(
            Get_Hitbox_Center(), fire_direction, game::EnemyConfig::kDarknessMonsterProjectileSpeed,
            this->enemy_Damage, game::EnemyConfig::kDarknessMonsterProjectileSprite
        );
        om_ref_.AddObject(projectile);
    }

    void DarknessMonster::Draw()
    {
        Vector2 direction = Vector2Normalize({ last_player_position_.x - this->Get_Hitbox_Center().x, last_player_position_.y - this->Get_Hitbox_Center().y });
        Facing_Direction facing_dir = Facing_Direction::DOWN; // Standardwert

        // Wähle die Logik zur Richtungsbestimmung basierend auf dem aktuellen Zustand
        if (ai_state_ == AIState::ATTACKING_RANGED)
        {
            // Nutze präzise 8 Richtungen für den Fernkampfangriff
            float angle = atan2(direction.y, direction.x) * (180.0f / PI);
            if (angle < 0) angle += 360;

            if (angle >= 337.5 || angle < 22.5) facing_dir = Facing_Direction::RIGHT;
            else if (angle >= 22.5 && angle < 67.5) facing_dir = Facing_Direction::DOWN_RIGHT;
            else if (angle >= 67.5 && angle < 112.5) facing_dir = Facing_Direction::DOWN;
            else if (angle >= 112.5 && angle < 157.5) facing_dir = Facing_Direction::DOWN_LEFT;
            else if (angle >= 157.5 && angle < 202.5) facing_dir = Facing_Direction::LEFT;
            else if (angle >= 202.5 && angle < 247.5) facing_dir = Facing_Direction::UP_LEFT;
            else if (angle >= 247.5 && angle < 292.5) facing_dir = Facing_Direction::UP;
            else if (angle >= 292.5 && angle < 337.5) facing_dir = Facing_Direction::UP_RIGHT;
        }
        else
        {
            // Nutze für alle anderen Zustände (Laufen, Nahkampf) die robustere 4-Richtungs-Logik
            if (fabs(direction.x) > fabs(direction.y)) {
                facing_dir = (direction.x > 0) ? Facing_Direction::RIGHT : Facing_Direction::LEFT;
            } else {
                facing_dir = (direction.y > 0) ? Facing_Direction::DOWN : Facing_Direction::UP;
            }
        }

        // Wähle die korrekte Animation basierend auf dem Zustand und der Richtung aus
        switch (ai_state_)
        {
            case AIState::CHASING_RANGED:
            case AIState::CHASING_MELEE:
                if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_walk_back_;
                else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_walk_front_;
                else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_walk_left_;
                else p_current_animation_ = &anim_walk_right_;
                break;
            case AIState::ATTACKING_MELEE:
                if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_melee_back_;
                else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_melee_front_;
                else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_melee_left_;
                else p_current_animation_ = &anim_melee_right_;
                break;
            case AIState::ATTACKING_RANGED:
                switch(facing_dir){
                    case Facing_Direction::UP: p_current_animation_ = &anim_ranged_up_; break;
                    case Facing_Direction::DOWN: p_current_animation_ = &anim_ranged_down_; break;
                    case Facing_Direction::LEFT: p_current_animation_ = &anim_ranged_left_; break;
                    case Facing_Direction::RIGHT: p_current_animation_ = &anim_ranged_right_; break;
                    case Facing_Direction::UP_LEFT: p_current_animation_ = &anim_ranged_up_left_; break;
                    case Facing_Direction::UP_RIGHT: p_current_animation_ = &anim_ranged_up_right_; break;
                    case Facing_Direction::DOWN_LEFT: p_current_animation_ = &anim_ranged_down_left_; break;
                    case Facing_Direction::DOWN_RIGHT: p_current_animation_ = &anim_ranged_down_right_; break;
                }
                break;
            case AIState::DYING:
                p_current_animation_ = &anim_death_;
                break;
        }

        if (p_current_animation_)
        {
            Vector2 draw_pos = {
                this->hitbox.x - game::EnemyConfig::kDarknessMonster_visual_offset.x,
                this->hitbox.y - game::EnemyConfig::kDarknessMonster_visual_offset.y
            };
            Color tint = { (unsigned char)this->tint_color.r, (unsigned char)this->tint_color.g, (unsigned char)this->tint_color.b, (unsigned char)(this->visibility_alpha * 255.0f) };

            p_current_animation_->Draw_Current_Frame(draw_pos, tint);

            if (this->is_animation_active_) {
                p_current_animation_->Next_Frame();
            } else {
                p_current_animation_->Reset();
            }
        }
    }
    void DarknessMonster::PlayHitSound() { /*  Add sound path */ }
    void DarknessMonster::PlayDeathSound() { /*  Add sound path */ }
}