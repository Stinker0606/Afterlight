#include "WoodSniper.h"
#include "../../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raymath.h"
#include "../EnemyProjectile.h"
#include "../core/Object_Manager.h"
#include "SoundManager.h"
#include "../../config_audio.h.in"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy
{
    WoodSniper::WoodSniper(Vector2 start_position, Object_Manager& om,  bool use_fog)
        : Enemy_Base_Class(
            "Wood Sniper",
            game::EnemyConfig::kWoodSniperHealth,
            game::EnemyConfig::kWoodSniperMovementSpeed,
            game::EnemyConfig::kWoodSniperDamage,
            game::EnemyConfig::kWoodSniperValue,
            game::EnemyConfig::kWoodSniperSpritePath,
            nullptr,
            start_position,
            game::EnemyConfig::kWoodSniperHitboxWidth,
            game::EnemyConfig::kWoodSniperHitboxHeight,
            game::EnemyConfig::kWoodSniperAttackCooldown,
            om
          ),
          anim_death_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperDeathPath, game::EnemyConfig::kWoodSniperDeathFrames, game::EnemyConfig::kWoodSniperDeathFramesPerLine, game::EnemyConfig::kWoodSniperDeathTimings, false),
          anim_attack_up_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackUpPath, game::EnemyConfig::kWoodSniperAttackUpFrames, game::EnemyConfig::kWoodSniperAttackUpFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_down_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackDownPath, game::EnemyConfig::kWoodSniperAttackDownFrames, game::EnemyConfig::kWoodSniperAttackDownFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackLeftPath, game::EnemyConfig::kWoodSniperAttackLeftFrames, game::EnemyConfig::kWoodSniperAttackLeftFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackRightPath, game::EnemyConfig::kWoodSniperAttackRightFrames, game::EnemyConfig::kWoodSniperAttackRightFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_up_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackUpLeftPath, game::EnemyConfig::kWoodSniperAttackUpLeftFrames, game::EnemyConfig::kWoodSniperAttackUpLeftFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_up_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackUpRightPath, game::EnemyConfig::kWoodSniperAttackUpRightFrames, game::EnemyConfig::kWoodSniperAttackUpRightFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_down_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackDownLeftPath, game::EnemyConfig::kWoodSniperAttackDownLeftFrames, game::EnemyConfig::kWoodSniperAttackDownLeftFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_attack_down_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperAttackDownRightPath, game::EnemyConfig::kWoodSniperAttackDownRightFrames, game::EnemyConfig::kWoodSniperAttackDownRightFramesPerLine, game::EnemyConfig::kWoodSniperAttackTimings, false),
          anim_reload_up_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadUpPath, game::EnemyConfig::kWoodSniperReloadUpFrames, game::EnemyConfig::kWoodSniperReloadUpFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_down_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadDownPath, game::EnemyConfig::kWoodSniperReloadDownFrames, game::EnemyConfig::kWoodSniperReloadDownFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadLeftPath, game::EnemyConfig::kWoodSniperReloadLeftFrames, game::EnemyConfig::kWoodSniperReloadLeftFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadRightPath, game::EnemyConfig::kWoodSniperReloadRightFrames, game::EnemyConfig::kWoodSniperReloadRightFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_up_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadUpLeftPath, game::EnemyConfig::kWoodSniperReloadUpLeftFrames, game::EnemyConfig::kWoodSniperReloadUpLeftFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_up_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadUpRightPath, game::EnemyConfig::kWoodSniperReloadUpRightFrames, game::EnemyConfig::kWoodSniperReloadUpRightFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_down_left_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadDownLeftPath, game::EnemyConfig::kWoodSniperReloadDownLeftFrames, game::EnemyConfig::kWoodSniperReloadDownLeftFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false),
          anim_reload_down_right_(game::EnemyConfig::kWoodSniperAnimSize, game::EnemyConfig::kWoodSniperReloadDownRightPath, game::EnemyConfig::kWoodSniperReloadDownRightFrames, game::EnemyConfig::kWoodSniperReloadDownRightFramesPerLine, game::EnemyConfig::kWoodSniperReloadTimings, false)
    {
        this->useFog = true;
        this->anim_state_ = AnimationState::RELOADING;
        this->p_current_animation_ = &anim_reload_down_;
        this->has_fired_ = false;
    }

    void WoodSniper::Update_AI(float delta_time, Vector2 player_position)
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
            case AnimationState::RELOADING:
                if (this->attack_Cooldown_Timer <= 0.0f)
                {
                    anim_state_ = AnimationState::ATTACKING;
                    anim_attack_up_.Reset(); anim_attack_down_.Reset(); anim_attack_left_.Reset(); anim_attack_right_.Reset();
                    anim_attack_up_left_.Reset(); anim_attack_up_right_.Reset(); anim_attack_down_left_.Reset(); anim_attack_down_right_.Reset();
                    has_fired_ = false;
                }
                break;

            case AnimationState::ATTACKING:
                if (p_current_animation_ != nullptr && p_current_animation_->Get_Current_Frame() == 5 && !has_fired_) {
                    this->Range_Attack();
                    has_fired_ = true;
                }

                if (p_current_animation_ != nullptr && p_current_animation_->Is_Finished()) {
                    anim_state_ = AnimationState::RELOADING;
                    this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
                    anim_reload_up_.Reset(); anim_reload_down_.Reset(); anim_reload_left_.Reset(); anim_reload_right_.Reset();
                    anim_reload_up_left_.Reset(); anim_reload_up_right_.Reset(); anim_reload_down_left_.Reset(); anim_reload_down_right_.Reset();
                }
                break;
        }
    }

    void WoodSniper::Range_Attack()
    {
        Vector2 fire_direction = Vector2Normalize({
            last_player_position_.x - this->Get_Hitbox_Center().x,
            last_player_position_.y - this->Get_Hitbox_Center().y
        });

        auto projectile = std::make_shared<game::Enemy_Projectile>(
            this->Get_Hitbox_Center(),
            fire_direction,
            game::EnemyConfig::kWoodSniperProjectileSpeed,
            this->enemy_Damage,
            game::EnemyConfig::kWoodSniperProjectileSpriteUp
        );
        SoundManager::GetInstance().PlaySfx("enemy_wood_sniper_shoot");
        om_ref_.AddObject(projectile);
    }

    void WoodSniper::PlayHitSound() {
        SoundManager::GetInstance().PlaySfx("enemy_wood_sniper_hit");
    }

    void WoodSniper::PlayDeathSound() {
        SoundManager::GetInstance().PlaySfx("enemy_wood_sniper_death");
    }

    void WoodSniper::PlayMoveSound() {}

    void WoodSniper::Draw()
    {
        if (game::EnemyConfig::kUseEnemyAnimations)
        {
            Vector2 direction = Vector2Normalize({ last_player_position_.x - this->Get_Hitbox_Center().x, last_player_position_.y - this->Get_Hitbox_Center().y });
            float angle = atan2(direction.y, direction.x) * (180.0f / PI);
            if (angle < 0) angle += 360;

            Facing_Direction facing_dir = Facing_Direction::DOWN;
            if (angle >= 337.5 || angle < 22.5) facing_dir = Facing_Direction::RIGHT;
            else if (angle >= 22.5 && angle < 67.5) facing_dir = Facing_Direction::DOWN_RIGHT;
            else if (angle >= 67.5 && angle < 112.5) facing_dir = Facing_Direction::DOWN;
            else if (angle >= 112.5 && angle < 157.5) facing_dir = Facing_Direction::DOWN_LEFT;
            else if (angle >= 157.5 && angle < 202.5) facing_dir = Facing_Direction::LEFT;
            else if (angle >= 202.5 && angle < 247.5) facing_dir = Facing_Direction::UP_LEFT;
            else if (angle >= 247.5 && angle < 292.5) facing_dir = Facing_Direction::UP;
            else if (angle >= 292.5 && angle < 337.5) facing_dir = Facing_Direction::UP_RIGHT;

            switch(anim_state_){
                case AnimationState::RELOADING:
                    switch(facing_dir){
                        case Facing_Direction::UP: p_current_animation_ = &anim_reload_up_; break;
                        case Facing_Direction::DOWN: p_current_animation_ = &anim_reload_down_; break;
                        case Facing_Direction::LEFT: p_current_animation_ = &anim_reload_left_; break;
                        case Facing_Direction::RIGHT: p_current_animation_ = &anim_reload_right_; break;
                        case Facing_Direction::UP_LEFT: p_current_animation_ = &anim_reload_up_left_; break;
                        case Facing_Direction::UP_RIGHT: p_current_animation_ = &anim_reload_up_right_; break;
                        case Facing_Direction::DOWN_LEFT: p_current_animation_ = &anim_reload_down_left_; break;
                        case Facing_Direction::DOWN_RIGHT: p_current_animation_ = &anim_reload_down_right_; break;
                    }
                break;
                case AnimationState::ATTACKING:
                    switch(facing_dir){
                        case Facing_Direction::UP: p_current_animation_ = &anim_attack_up_; break;
                        case Facing_Direction::DOWN: p_current_animation_ = &anim_attack_down_; break;
                        case Facing_Direction::LEFT: p_current_animation_ = &anim_attack_left_; break;
                        case Facing_Direction::RIGHT: p_current_animation_ = &anim_attack_right_; break;
                        case Facing_Direction::UP_LEFT: p_current_animation_ = &anim_attack_up_left_; break;
                        case Facing_Direction::UP_RIGHT: p_current_animation_ = &anim_attack_up_right_; break;
                        case Facing_Direction::DOWN_LEFT: p_current_animation_ = &anim_attack_down_left_; break;
                        case Facing_Direction::DOWN_RIGHT: p_current_animation_ = &anim_attack_down_right_; break;
                    }
                break;
                case AnimationState::DYING:
                    p_current_animation_ = &anim_death_;
                break;
            }

            if (p_current_animation_)
            {
                Vector2 draw_pos = {
                    this->hitbox.x - game::EnemyConfig::kWoodSniper_visual_offset.x,
                    this->hitbox.y - game::EnemyConfig::kWoodSniper_visual_offset.y
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