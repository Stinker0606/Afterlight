#include "DrownedSniper.h"
#include "../../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raymath.h"
#include "../EnemyProjectile.h"
#include "../../core/Object_Manager.h"
#include "raylib.h"

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
          anim_death_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperDeathPath, game::EnemyConfig::kDrownedSniperDeathFrames, game::EnemyConfig::kDrownedSniperDeathFramesPerLine),
          anim_attack_up_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpPath, game::EnemyConfig::kDrownedSniperAttackUpFrames, game::EnemyConfig::kDrownedSniperAttackUpFramesPerLine),
          anim_attack_down_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownPath, game::EnemyConfig::kDrownedSniperAttackDownFrames, game::EnemyConfig::kDrownedSniperAttackDownFramesPerLine),
          anim_attack_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackLeftPath, game::EnemyConfig::kDrownedSniperAttackLeftFrames, game::EnemyConfig::kDrownedSniperAttackLeftFramesPerLine),
          anim_attack_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackRightPath, game::EnemyConfig::kDrownedSniperAttackRightFrames, game::EnemyConfig::kDrownedSniperAttackRightFramesPerLine),
          anim_attack_up_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpLeftPath, game::EnemyConfig::kDrownedSniperAttackUpLeftFrames, game::EnemyConfig::kDrownedSniperAttackUpLeftFramesPerLine),
          anim_attack_up_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpRightPath, game::EnemyConfig::kDrownedSniperAttackUpRightFrames, game::EnemyConfig::kDrownedSniperAttackUpRightFramesPerLine),
          anim_attack_down_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownLeftPath, game::EnemyConfig::kDrownedSniperAttackDownLeftFrames, game::EnemyConfig::kDrownedSniperAttackDownLeftFramesPerLine),
          anim_attack_down_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownRightPath, game::EnemyConfig::kDrownedSniperAttackDownRightFrames, game::EnemyConfig::kDrownedSniperAttackDownRightFramesPerLine),
          anim_reload_up_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpPath, game::EnemyConfig::kDrownedSniperReloadUpFrames, game::EnemyConfig::kDrownedSniperReloadUpFramesPerLine),
          anim_reload_down_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownPath, game::EnemyConfig::kDrownedSniperReloadDownFrames, game::EnemyConfig::kDrownedSniperReloadDownFramesPerLine),
          anim_reload_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadLeftPath, game::EnemyConfig::kDrownedSniperReloadLeftFrames, game::EnemyConfig::kDrownedSniperReloadLeftFramesPerLine),
          anim_reload_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadRightPath, game::EnemyConfig::kDrownedSniperReloadRightFrames, game::EnemyConfig::kDrownedSniperReloadRightFramesPerLine),
          anim_reload_up_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpLeftPath, game::EnemyConfig::kDrownedSniperReloadUpLeftFrames, game::EnemyConfig::kDrownedSniperReloadUpLeftFramesPerLine),
          anim_reload_up_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadUpRightPath, game::EnemyConfig::kDrownedSniperReloadUpRightFrames, game::EnemyConfig::kDrownedSniperReloadUpRightFramesPerLine),
          anim_reload_down_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownLeftPath, game::EnemyConfig::kDrownedSniperReloadDownLeftFrames, game::EnemyConfig::kDrownedSniperReloadDownLeftFramesPerLine),
          anim_reload_down_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperReloadDownRightPath, game::EnemyConfig::kDrownedSniperReloadDownRightFrames, game::EnemyConfig::kDrownedSniperReloadDownRightFramesPerLine)
    {
        this->useFog = true;
        // Startzustand ist RELOADING.
        this->anim_state_ = AnimationState::RELOADING;
        this->p_current_animation_ = &anim_reload_down_;
        this->attack_animation_timer_ = 0.0f;
        this->reload_timer_ = 0.0f;
        this->has_fired_ = false;
    }

    void DrownedSniper::Update_AI(float delta_time, Vector2 player_position)
    {
        if (this->enemy_Health <= 0 && anim_state_ != AnimationState::DYING) {
            anim_state_ = AnimationState::DYING;
        }
        if (anim_state_ == AnimationState::DYING) {
            return; // Im Todeszustand keine weitere Logik ausführen
        }

        last_player_position_ = player_position;
        Enemy_Base_Class::Tick(delta_time);

        switch (anim_state_)
        {
            case AnimationState::RELOADING:
                // Wenn der Cooldown abgelaufen ist, beginnt die Nachlade/Ziel-Phase
                if (this->attack_Cooldown_Timer <= 0.0f)
                {
                    reload_timer_ += delta_time;
                    // Nach 1.5 Sekunden Zielen, in den Angriffs-Modus wechseln
                    if (reload_timer_ >= 8.0f)
                    {
                        anim_state_ = AnimationState::ATTACKING;
                        attack_animation_timer_ = 3.6f; // Dauer der Schuss-Animation
                        has_fired_ = false; // Zurücksetzen für den nächsten Schuss
                        reload_timer_ = 0.0f; // Nachlade-Timer zurücksetzen
                    }
                }
                break;

            case AnimationState::ATTACKING:
                attack_animation_timer_ -= delta_time;
                // Feuere das Projektil nach 0.2 Sekunden in der Schuss-Animation ab
                if (attack_animation_timer_ <= 1.2f && !has_fired_)
                {
                    this->Range_Attack();
                    has_fired_ = true;
                }

                // Wenn die Schuss-Animation vorbei ist...
                if (attack_animation_timer_ <= 0.0f)
                {
                    // ...gehe zurück ins Nachladen und starte den globalen Cooldown.
                    anim_state_ = AnimationState::RELOADING;
                    this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
                }
                break;
            case AnimationState::DYING:
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

        om_ref_.AddObject(projectile);
    }

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
                p_current_animation_->Draw_Current_Frame(draw_pos, Fade(WHITE, this->visibility_alpha));

                if(anim_state_ == AnimationState::RELOADING && this->attack_Cooldown_Timer <= 0.0f) {
                    p_current_animation_->First_Frame(); // Bleibe auf dem ersten Frame, während gezielt wird
                } else {
                    p_current_animation_->Next_Frame(); // Spiele die Animation normal ab
                }
            }
        }
        else
        {
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}