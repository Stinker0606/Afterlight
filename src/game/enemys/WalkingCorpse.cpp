#include "WalkingCorpse.h"
#include "../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "raymath.h"
#include "../../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raylib.h"
#include "SoundManager.h"
#include "../../config_audio.h.in"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy
{
    WalkingCorpse::WalkingCorpse(Vector2 start_position, Object_Manager& om, bool use_fog)
        : Enemy_Base_Class(
            "Walking Corpse",
            game::EnemyConfig::kWalkingCorpseHealth,
            game::EnemyConfig::kWalkingCorpseMovementSpeed,
            game::EnemyConfig::kWalkingCorpseDamage,
            game::EnemyConfig::kWalkingCorpseValue,
            game::EnemyConfig::kWalkingCorpseSpritePath,
            nullptr,
            start_position,
            game::EnemyConfig::kWalkingCorpseHitboxWidth,
            game::EnemyConfig::kWalkingCorpseHitboxHeight,
            game::EnemyConfig::kWalkingCorpseAttackCooldown,
            om
          ),
          anim_walk_front_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseWalkFrontPath, game::EnemyConfig::kWalkingCorpseWalkFrontFrames, game::EnemyConfig::kWalkingCorpseWalkFrontFramesPerLine, game::EnemyConfig::kWalkingCorpseWalkTimings, true),
          anim_walk_back_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseWalkBackPath, game::EnemyConfig::kWalkingCorpseWalkBackFrames, game::EnemyConfig::kWalkingCorpseWalkBackFramesPerLine, game::EnemyConfig::kWalkingCorpseWalkTimings, true),
          anim_walk_left_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseWalkLeftPath, game::EnemyConfig::kWalkingCorpseWalkLeftFrames, game::EnemyConfig::kWalkingCorpseWalkLeftFramesPerLine, game::EnemyConfig::kWalkingCorpseWalkTimings, true),
          anim_walk_right_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseWalkRightPath, game::EnemyConfig::kWalkingCorpseWalkRightFrames, game::EnemyConfig::kWalkingCorpseWalkRightFramesPerLine, game::EnemyConfig::kWalkingCorpseWalkTimings, true),
          anim_attack_front_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseAttackFrontPath, game::EnemyConfig::kWalkingCorpseAttackFrontFrames, game::EnemyConfig::kWalkingCorpseAttackFrontFramesPerLine, game::EnemyConfig::kWalkingCorpseAttackTimings, false),
          anim_attack_back_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseAttackBackPath, game::EnemyConfig::kWalkingCorpseAttackBackFrames, game::EnemyConfig::kWalkingCorpseAttackBackFramesPerLine, game::EnemyConfig::kWalkingCorpseAttackTimings, false),
          anim_attack_left_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseAttackLeftPath, game::EnemyConfig::kWalkingCorpseAttackLeftFrames, game::EnemyConfig::kWalkingCorpseAttackLeftFramesPerLine, game::EnemyConfig::kWalkingCorpseAttackTimings, false),
          anim_attack_right_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseAttackRightPath, game::EnemyConfig::kWalkingCorpseAttackRightFrames, game::EnemyConfig::kWalkingCorpseAttackRightFramesPerLine, game::EnemyConfig::kWalkingCorpseAttackTimings, false),
          anim_death_(game::EnemyConfig::kWalkingCorpseAnimSize, game::EnemyConfig::kWalkingCorpseDeathPath, game::EnemyConfig::kWalkingCorpseDeathFrames, game::EnemyConfig::kWalkingCorpseDeathFramesPerLine, game::EnemyConfig::kWalkingCorpseDeathTimings, false)
    {
        this->useFog = true;
        this->anim_state_ = AnimationState::WALKING;
        this->p_current_animation_ = &anim_walk_front_;
        this->melee_hitbox_spawned_ = false;
    }

    void WalkingCorpse::Update_AI(float delta_time, Vector2 player_position)
    {
        if (anim_state_ != AnimationState::DYING && this->enemy_Health <= 0) {
            anim_state_ = AnimationState::DYING;
            anim_death_.Reset();
        }
        if (anim_state_ == AnimationState::DYING) {
            if (anim_death_.Is_Finished()) {
                this->Mark_For_Destruction();
            }
            return;
        }

        last_player_position_ = player_position;
        Enemy_Base_Class::Tick(delta_time);

        switch (anim_state_)
        {
            case AnimationState::WALKING:
                Pathfinding(player_position.x, player_position.y, delta_time);
            if (Vector2Distance(Get_Hitbox_Center(), player_position) <= game::EnemyConfig::kWalkingCorpseAttackRange && this->attack_Cooldown_Timer <= 0.0f)
            {
                anim_state_ = AnimationState::ATTACKING;
                melee_hitbox_spawned_ = false;
                anim_attack_front_.Reset();
                anim_attack_back_.Reset();
                anim_attack_left_.Reset();
                anim_attack_right_.Reset();
            }
            break;

            case AnimationState::ATTACKING:
                if (p_current_animation_ && p_current_animation_->Get_Current_Frame() == 3 && !melee_hitbox_spawned_) {
                    this->Melee_Attack();
                    melee_hitbox_spawned_ = true;
                }
            if (p_current_animation_ && p_current_animation_->Is_Finished()) {
                anim_state_ = AnimationState::WALKING;
                this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
            }
            break;
        }
    }

    void WalkingCorpse::Melee_Attack()
    {
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        float sweep_width = 32.0f;
        float sweep_height = 48.0f;
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 13.0f;

        if (fabs(direction.x) > fabs(direction.y))
        {
            hitbox_width = sweep_width;
            hitbox_height = sweep_height;
            if (direction.x > 0) {
                hitbox_pos = { enemy_center.x + offset, enemy_center.y - hitbox_height / 2 };
            } else {
                hitbox_pos = { enemy_center.x - offset - hitbox_width, enemy_center.y - hitbox_height / 2 };
            }
        }
        else
        {
            hitbox_width = sweep_height;
            hitbox_height = sweep_width;
            if (direction.y > 0) {
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y + offset };
            } else {
                hitbox_pos = { enemy_center.x - hitbox_width / 2, enemy_center.y - offset - hitbox_height };
            }
        }

        auto sweep_hitbox = std::make_shared<MeleeHitbox>(
            Rectangle{ hitbox_pos.x, hitbox_pos.y, hitbox_width, hitbox_height },
            2.0f,
            this->enemy_Damage,
            Collision_Type::ENEMY
        );

        SoundManager::GetInstance().PlaySfx("enemy_corpse_attack");
        om_ref_.AddObject(sweep_hitbox);
    }

    void WalkingCorpse::Range_Attack() {}

    void WalkingCorpse::PlayHitSound() {
        SoundManager::GetInstance().PlaySfx("enemy_corpse_hit");
    }

    void WalkingCorpse::PlayDeathSound() {
        SoundManager::GetInstance().PlaySfx("enemy_corpse_death");
    }

    void WalkingCorpse::PlayMoveSound() {
        SoundManager::GetInstance().PlaySfx("enemy_corpse_move", 1);
    }

    void WalkingCorpse::Draw()
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
                case AnimationState::WALKING:
                    if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_walk_back_;
                    else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_walk_front_;
                    else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_walk_left_;
                    else p_current_animation_ = &anim_walk_right_;
                break;
                case AnimationState::ATTACKING:
                    if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_attack_back_;
                    else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_attack_front_;
                    else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_attack_left_;
                    else p_current_animation_ = &anim_attack_right_;
                break;
                case AnimationState::DYING:
                    p_current_animation_ = &anim_death_;
                break;
            }

            if (p_current_animation_)
            {
                Vector2 draw_pos = {
                    this->hitbox.x - game::EnemyConfig::kWalkingCorpse_visual_offset.x,
                    this->hitbox.y - game::EnemyConfig::kWalkingCorpse_visual_offset.y
                };
                Color tint = { 255, 255, 255, (unsigned char)(this->visibility_alpha * 255.0f) };
                p_current_animation_->Draw_Current_Frame(draw_pos, tint);

                if (this->is_animation_active_) {
                    p_current_animation_->Next_Frame();
                } else {
                    p_current_animation_->Reset();
                }
            }
        }
        else
        {
            // FALLBACK, WENN ANIMATIONEN DEAKTIVIERT SIND
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}