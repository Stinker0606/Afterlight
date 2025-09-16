#include "Mimic.h"
#include "../core/Object_Manager.h"
#include "../interactables/MeleeHitbox.h"
#include "raymath.h"
#include "../../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raylib.h"
#include "SoundManager.h"
#include "../../config_audio.h.in"
#include "Store.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace enemy
{
    Mimic::Mimic(Vector2 start_position, Object_Manager& om, bool use_fog)
        : Enemy_Base_Class(
            "Mimic",
            game::EnemyConfig::kMimicHealth,
            game::EnemyConfig::kMimicMovementSpeed,
            game::EnemyConfig::kMimicDamage,
            game::EnemyConfig::kMimicValue,
            game::EnemyConfig::kMimicStatueSpritePath,
            nullptr,
            start_position,
            30,
            40,
            game::EnemyConfig::kMimicAttackCooldown,
            om
          ),
          anim_transform_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicTransformPath, game::EnemyConfig::kMimicTransformFrames, game::EnemyConfig::kMimicTransformFramesPerLine, game::EnemyConfig::kMimicTransformTimings, false),
          anim_death_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicDeathPath, game::EnemyConfig::kMimicDeathFrames, game::EnemyConfig::kMimicDeathFramesPerLine, game::EnemyConfig::kMimicDeathTimings, false),
          anim_walk_front_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicWalkFrontPath, game::EnemyConfig::kMimicWalkFrontFrames, game::EnemyConfig::kMimicWalkFrontFramesPerLine, game::EnemyConfig::kMimicWalkTimings, true),
          anim_walk_back_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicWalkBackPath, game::EnemyConfig::kMimicWalkBackFrames, game::EnemyConfig::kMimicWalkBackFramesPerLine, game::EnemyConfig::kMimicWalkTimings, true),
          anim_walk_left_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicWalkLeftPath, game::EnemyConfig::kMimicWalkLeftFrames, game::EnemyConfig::kMimicWalkLeftFramesPerLine, game::EnemyConfig::kMimicWalkTimings, true),
          anim_walk_right_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicWalkRightPath, game::EnemyConfig::kMimicWalkRightFrames, game::EnemyConfig::kMimicWalkRightFramesPerLine, game::EnemyConfig::kMimicWalkTimings, true),
          anim_attack_front_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicAttackFrontPath, game::EnemyConfig::kMimicAttackFrontFrames, game::EnemyConfig::kMimicAttackFrontFramesPerLine, game::EnemyConfig::kMimicAttackTimings, false),
          anim_attack_back_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicAttackBackPath, game::EnemyConfig::kMimicAttackBackFrames, game::EnemyConfig::kMimicAttackBackFramesPerLine, game::EnemyConfig::kMimicAttackTimings, false),
          anim_attack_left_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicAttackLeftPath, game::EnemyConfig::kMimicAttackLeftFrames, game::EnemyConfig::kMimicAttackLeftFramesPerLine, game::EnemyConfig::kMimicAttackTimings, false),
          anim_attack_right_(game::EnemyConfig::kMimicAnimSize, game::EnemyConfig::kMimicAttackRightPath, game::EnemyConfig::kMimicAttackRightFrames, game::EnemyConfig::kMimicAttackRightFramesPerLine, game::EnemyConfig::kMimicAttackTimings, false)
    {
        this->useFog = false;
        this->current_state_ = MimicState::TRANSFORMING;
        this->p_current_animation_ = &anim_transform_;
        this->melee_hitbox_spawned_ = false;

        SoundManager::GetInstance().PlaySfx("mimic_transform");
        anim_transform_.Reset();
    }

    void Mimic::Update_AI(float delta_time, Vector2 player_position)
    {
        last_player_position_ = player_position;

        if (current_state_ != MimicState::DYING && this->enemy_Health <= 0) {
            current_state_ = MimicState::DYING;
            anim_death_.Reset();
        }
        if (current_state_ == MimicState::DYING) {
            if (anim_death_.Is_Finished()) {
                this->Mark_For_Destruction();
            }
            return;
        }

        switch (current_state_)
        {
            case MimicState::TRANSFORMING:
                if (anim_transform_.Is_Finished())
                {
                    current_state_ = MimicState::WALKING;
                }
                break;

            case MimicState::WALKING:
                Enemy_Base_Class::Tick(delta_time);
                Pathfinding(player_position, delta_time, 16);
                if (Vector2Distance(Get_Hitbox_Center(), player_position) <= game::EnemyConfig::kMimicAttackRange && this->attack_Cooldown_Timer <= 0.0f)
                {
                    current_state_ = MimicState::ATTACKING;
                    melee_hitbox_spawned_ = false;
                    anim_attack_front_.Reset();
                    anim_attack_back_.Reset();
                    anim_attack_left_.Reset();
                    anim_attack_right_.Reset();
                }
                break;

            case MimicState::ATTACKING:
                Enemy_Base_Class::Tick(delta_time);
                if (p_current_animation_ && p_current_animation_->Get_Current_Frame() == 4 && !melee_hitbox_spawned_) {
                    this->Melee_Attack();
                    melee_hitbox_spawned_ = true;
                }
                if (p_current_animation_ && p_current_animation_->Is_Finished()) {
                    current_state_ = MimicState::WALKING;
                    this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
                }
                break;
        }
    }

    void Mimic::Melee_Attack()
    {
        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;

        float sweep_width = 34.0f;
        float sweep_height = 48.0f;
        float hitbox_width, hitbox_height;
        Vector2 hitbox_pos;

        Vector2 enemy_center = this->Get_Hitbox_Center();
        Vector2 direction = Vector2Normalize({ last_player_position_.x - enemy_center.x, last_player_position_.y - enemy_center.y });

        float offset = 14.0f;

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

        SoundManager::GetInstance().PlaySfx("mimic_attack");
        om_ref_.AddObject(sweep_hitbox);
    }

    void Mimic::Take_Damage(int damage)
    {
        if (current_state_ != MimicState::TRANSFORMING)
        {
            Enemy_Base_Class::Take_Damage(damage);
        }
    }

    void Mimic::PlayHitSound() { SoundManager::GetInstance().PlaySfx("mimic_hit"); }
    void Mimic::PlayDeathSound() { SoundManager::GetInstance().PlaySfx("mimic_death"); }

    void Mimic::Draw()
    {
        if (game::EnemyConfig::kUseEnemyAnimations)
        {
            switch (current_state_)
            {
                case MimicState::TRANSFORMING:
                    p_current_animation_ = &anim_transform_;
                    break;
                case MimicState::DYING:
                    p_current_animation_ = &anim_death_;
                    break;
                case MimicState::WALKING:
                case MimicState::ATTACKING:
                {
                    Vector2 direction = Vector2Normalize({ last_player_position_.x - this->Get_Hitbox_Center().x, last_player_position_.y - this->Get_Hitbox_Center().y });
                    float angle = atan2(direction.y, direction.x) * (180.0f / PI);
                    if (angle < 0) angle += 360;
                    Facing_Direction facing_dir = Facing_Direction::DOWN;
                    if (angle >= 45 && angle < 135) facing_dir = Facing_Direction::DOWN;
                    else if (angle >= 135 && angle < 225) facing_dir = Facing_Direction::LEFT;
                    else if (angle >= 225 && angle < 315) facing_dir = Facing_Direction::UP;
                    else facing_dir = Facing_Direction::RIGHT;

                    if (current_state_ == MimicState::WALKING) {
                        if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_walk_back_;
                        else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_walk_front_;
                        else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_walk_left_;
                        else p_current_animation_ = &anim_walk_right_;
                    } else { // ATTACKING
                        if (facing_dir == Facing_Direction::UP) p_current_animation_ = &anim_attack_back_;
                        else if (facing_dir == Facing_Direction::DOWN) p_current_animation_ = &anim_attack_front_;
                        else if (facing_dir == Facing_Direction::LEFT) p_current_animation_ = &anim_attack_left_;
                        else p_current_animation_ = &anim_attack_right_;
                    }
                    break;
                }
            }

            if (p_current_animation_)
            {
                Vector2 draw_pos = {
                    this->hitbox.x - game::EnemyConfig::kMimic_visual_offset.x,
                    this->hitbox.y - game::EnemyConfig::kMimic_visual_offset.y
                };
                Color tint = { (unsigned char)this->tint_color.r, (unsigned char)this->tint_color.g, (unsigned char)this->tint_color.b, (unsigned char)(this->visibility_alpha * 255.0f) };
                p_current_animation_->Draw_Current_Frame(draw_pos, tint);
                p_current_animation_->Next_Frame();
            }
        }
        else
        {
            DrawTextureEx(this->sprite, {this->hitbox.x, this->hitbox.y}, 0,2,Fade(WHITE, this->visibility_alpha));
        }
    }
}