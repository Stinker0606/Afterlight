#include "DrownedSniper.h"
#include "../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raymath.h"
#include "../EnemyProjectile.h"
#include "../core/Object_Manager.h"
#include "raylib.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy
{
    DrownedSniper::DrownedSniper(Vector2 start_position, Object_Manager& om,  bool use_fog)
        : Enemy_Base_Class(
            "Drowned Sniper",
            game::EnemyConfig::kDrownedSniperHealth,
            game::EnemyConfig::kDrownedSniperMovementSpeed,
            game::EnemyConfig::kDrownedSniperDamage,
            game::EnemyConfig::kDrownedSniperValue,
            game::EnemyConfig::kDrownedSniperSpritePath,
            nullptr,
            start_position,
            game::EnemyConfig::kDrownedSniperHitboxWidth,
            game::EnemyConfig::kDrownedSniperHitboxHeight,
            game::EnemyConfig::kDrownedSniperAttackCooldown,
            om
          ),
          // Initialisierung der Animationen
          anim_death_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperDeathPath, game::EnemyConfig::kDrownedSniperDeathFrames, game::EnemyConfig::kDrownedSniperDeathFramesPerLine, game::EnemyConfig::kDrownedSniperDeathTimings),
          anim_attack_up_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpPath, game::EnemyConfig::kDrownedSniperAttackUpFrames, game::EnemyConfig::kDrownedSniperAttackUpFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_down_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownPath, game::EnemyConfig::kDrownedSniperAttackDownFrames, game::EnemyConfig::kDrownedSniperAttackDownFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackLeftPath, game::EnemyConfig::kDrownedSniperAttackLeftFrames, game::EnemyConfig::kDrownedSniperAttackLeftFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackRightPath, game::EnemyConfig::kDrownedSniperAttackRightFrames, game::EnemyConfig::kDrownedSniperAttackRightFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_up_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpLeftPath, game::EnemyConfig::kDrownedSniperAttackUpLeftFrames, game::EnemyConfig::kDrownedSniperAttackUpLeftFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_up_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpRightPath, game::EnemyConfig::kDrownedSniperAttackUpRightFrames, game::EnemyConfig::kDrownedSniperAttackUpRightFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_down_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownLeftPath, game::EnemyConfig::kDrownedSniperAttackDownLeftFrames, game::EnemyConfig::kDrownedSniperAttackDownLeftFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_attack_down_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownRightPath, game::EnemyConfig::kDrownedSniperAttackDownRightFrames, game::EnemyConfig::kDrownedSniperAttackDownRightFramesPerLine, game::EnemyConfig::kDrownedSniperAttackTimings),
          anim_reload_up_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpPath, game::EnemyConfig::kDrownedSniperReloadUpFrames, game::EnemyConfig::kDrownedSniperReloadUpFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_down_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownPath, game::EnemyConfig::kDrownedSniperReloadDownFrames, game::EnemyConfig::kDrownedSniperReloadDownFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadLeftPath, game::EnemyConfig::kDrownedSniperReloadLeftFrames, game::EnemyConfig::kDrownedSniperReloadLeftFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadRightPath, game::EnemyConfig::kDrownedSniperReloadRightFrames, game::EnemyConfig::kDrownedSniperReloadRightFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_up_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpLeftPath, game::EnemyConfig::kDrownedSniperReloadUpLeftFrames, game::EnemyConfig::kDrownedSniperReloadUpLeftFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_up_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpRightPath, game::EnemyConfig::kDrownedSniperReloadUpRightFrames, game::EnemyConfig::kDrownedSniperReloadUpRightFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_down_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownLeftPath, game::EnemyConfig::kDrownedSniperReloadDownLeftFrames, game::EnemyConfig::kDrownedSniperReloadDownLeftFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings),
          anim_reload_down_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownRightPath, game::EnemyConfig::kDrownedSniperReloadDownRightFrames, game::EnemyConfig::kDrownedSniperReloadDownRightFramesPerLine, game::EnemyConfig::kDrownedSniperReloadTimings)
    {
        this->useFog = true;
        this->anim_state_ = AnimationState::RELOADING;
        this->p_current_animation_ = &anim_reload_down_;
        this->reload_timer_ = 0.0f;
        this->has_fired_ = false;
    }

    void DrownedSniper::Update_AI(float delta_time, Vector2 player_position)
{
    // --- 1. Todes-Logik (hat immer Priorität) ---
    if (anim_state_ != AnimationState::DYING && this->enemy_Health <= 0) {
        anim_state_ = AnimationState::DYING;
        anim_death_.Reset(); // Setze die Todes-Animation spezifisch zurück
    }

    if (anim_state_ == AnimationState::DYING) {
        if (anim_death_.Is_Finished()) {
            this->Mark_For_Destruction();
        }
        return; // Mache nichts anderes, wenn der Gegner stirbt.
    }

    // --- 2. Timer und Position aktualisieren ---
    last_player_position_ = player_position;
    Enemy_Base_Class::Tick(delta_time); // Zählt den Cooldown-Timer herunter

    // --- 3. Zustands-Logik ---
    switch (anim_state_)
    {
        case AnimationState::RELOADING:
            // Wenn der Cooldown abgelaufen ist, können wir angreifen.
            if (this->attack_Cooldown_Timer <= 0.0f && Vector2Distance(Get_Hitbox_Center(), player_position) <= game::EnemyConfig::kDrownedSniperAttackRange)
            {
                anim_state_ = AnimationState::ATTACKING;
                // WICHTIG: Setze die Angriffs-Animationen zurück, damit sie von vorne beginnen.
                // Da die Richtung erst im Draw-Call bestimmt wird, müssen wir hier alle zurücksetzen.
                anim_attack_up_.Reset(); anim_attack_down_.Reset(); anim_attack_left_.Reset(); anim_attack_right_.Reset();
                anim_attack_up_left_.Reset(); anim_attack_up_right_.Reset(); anim_attack_down_left_.Reset(); anim_attack_down_right_.Reset();
                has_fired_ = false;
            }
            break;

        case AnimationState::ATTACKING:
            // Feuere den Schuss bei Frame 5 ab (Zählung 0-11).
            if (p_current_animation_ != nullptr && p_current_animation_->Get_Current_Frame() == 5 && !has_fired_) {
                this->Range_Attack();
                has_fired_ = true;
            }

            // Wenn die Angriffs-Animation FERTIG ist, gehe zurück zum Nachladen.
            if (p_current_animation_ != nullptr && p_current_animation_->Is_Finished()) {
                anim_state_ = AnimationState::RELOADING;
                this->attack_Cooldown_Timer = this->attack_Cooldown_Duration; // Starte den Cooldown.
                // Setze die Reload-Animationen zurück.
                anim_reload_up_.Reset(); anim_reload_down_.Reset(); anim_reload_left_.Reset(); anim_reload_right_.Reset();
                anim_reload_up_left_.Reset(); anim_reload_up_right_.Reset(); anim_reload_down_left_.Reset(); anim_reload_down_right_.Reset();
            }
            break;
    }
}

    void DrownedSniper::Range_Attack()
    {
        Vector2 fire_direction = Vector2Normalize({
            last_player_position_.x - this->Get_Hitbox_Center().x,
            last_player_position_.y - this->Get_Hitbox_Center().y
        });

        auto projectile = std::make_shared<game::Enemy_Projectile>(
            this->Get_Hitbox_Center(),
            fire_direction,
            game::EnemyConfig::kDrownedSniperProjectileSpeed,
            this->enemy_Damage,
            game::EnemyConfig::kDrownedSniperProjectileSpriteUp
        );
        SoundManager::GetInstance().PlaySfx("enemy_sniper_shoot");
        om_ref_.AddObject(projectile);
    }

    void DrownedSniper::PlayHitSound() {
        SoundManager::GetInstance().PlaySfx("enemy_sniper_hit");
    }

    void DrownedSniper::PlayDeathSound() {
        SoundManager::GetInstance().PlaySfx("enemy_sniper_death");
    }

    void DrownedSniper::PlayMoveSound() {}

    void DrownedSniper::Draw()
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
                    this->hitbox.x - game::EnemyConfig::kDrownedSniper_visual_offset.x,
                    this->hitbox.y - game::EnemyConfig::kDrownedSniper_visual_offset.y
                };
                Color tint = { 255, 255, 255, (unsigned char)(this->visibility_alpha * 255.0f) };
                p_current_animation_->Draw_Current_Frame(draw_pos, tint);

                if (this->is_animation_active_) {
                    if(anim_state_ == AnimationState::RELOADING && this->attack_Cooldown_Timer <= 0.0f) {
                        // Bleibe auf dem letzten Frame der Reload-Animation stehen, während gezielt wird
                        // (Oder setze sie zurück, je nach gewünschtem Verhalten)
                    } else {
                        p_current_animation_->Next_Frame();
                    }
                } else {
                    p_current_animation_->Reset();
                }
            }
        }
        else
        {
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}