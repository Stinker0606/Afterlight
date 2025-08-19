#include "DrownedSniper.h"
#include "../../config_enemies.h.in"
#include "../FacingDirection.h"
#include "raymath.h"
#include "../EnemyProjectile.h"
#include "../../core/Object_Manager.h"

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
          anim_idle_(game::EnemyConfig::kDrownedSniperAnimSize, "assets/graphics/kRaoKr_imresizer.png", 1, 1), // Platzhalter
          anim_death_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperDeathPath, game::EnemyConfig::kDrownedSniperDeathFrames, game::EnemyConfig::kDrownedSniperDeathFramesPerLine),
          anim_attack_up_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpPath, game::EnemyConfig::kDrownedSniperAttackUpFrames, game::EnemyConfig::kDrownedSniperAttackUpFramesPerLine),
          anim_attack_down_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownPath, game::EnemyConfig::kDrownedSniperAttackDownFrames, game::EnemyConfig::kDrownedSniperAttackDownFramesPerLine),
          anim_attack_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackLeftPath, game::EnemyConfig::kDrownedSniperAttackLeftFrames, game::EnemyConfig::kDrownedSniperAttackLeftFramesPerLine),
          anim_attack_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackRightPath, game::EnemyConfig::kDrownedSniperAttackRightFrames, game::EnemyConfig::kDrownedSniperAttackRightFramesPerLine),
          anim_attack_up_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpLeftPath, game::EnemyConfig::kDrownedSniperAttackUpLeftFrames, game::EnemyConfig::kDrownedSniperAttackUpLeftFramesPerLine),
          anim_attack_up_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackUpRightPath, game::EnemyConfig::kDrownedSniperAttackUpRightFrames, game::EnemyConfig::kDrownedSniperAttackUpRightFramesPerLine),
          anim_attack_down_left_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownLeftPath, game::EnemyConfig::kDrownedSniperAttackDownLeftFrames, game::EnemyConfig::kDrownedSniperAttackDownLeftFramesPerLine),
          anim_attack_down_right_(game::EnemyConfig::kDrownedSniperAnimSize, game::EnemyConfig::kDrownedSniperAttackDownRightPath, game::EnemyConfig::kDrownedSniperAttackDownRightFrames, game::EnemyConfig::kDrownedSniperAttackDownRightFramesPerLine)
    {
        this->useFog = true;
        this->anim_state_ = AnimationState::IDLE;
        this->p_current_animation_ = &anim_idle_;
        this->attack_animation_timer_ = 0.0f;
    }

    void DrownedSniper::Update_AI(float delta_time, Vector2 player_position)
    {
        if (anim_state_ == AnimationState::DYING) return;
        if (this->enemy_Health <= 0) {
            anim_state_ = AnimationState::DYING;
            return;
        }

        last_player_position_ = player_position;
        Enemy_Base_Class::Tick(delta_time);

        if (attack_animation_timer_ > 0.0f) {
            attack_animation_timer_ -= delta_time;
            if (attack_animation_timer_ <= 0.0f) {
                anim_state_ = AnimationState::IDLE;
            }
        }
        else if (this->attack_Cooldown_Timer <= 0.0f)
        {
            anim_state_ = AnimationState::ATTACKING;
            attack_animation_timer_ = 0.6f; // Dauer der Angriffs-Animation
            this->Range_Attack();
            this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
        }
    }

    void DrownedSniper::Range_Attack()
    {
        // 1. Berechne die Richtung zum Spieler.
        Vector2 fire_direction = Vector2Normalize({
            last_player_position_.x - this->Get_Hitbox_Center().x,
            last_player_position_.y - this->Get_Hitbox_Center().y
        });

        // 2. Erstelle ein neues Projektil.
        auto projectile = std::make_shared<game::Enemy_Projectile>(
            this->Get_Hitbox_Center(),                          // Startet in der Mitte des Snipers
            fire_direction,                                     // Fliegt in Richtung Spieler
            game::EnemyConfig::kDrownedSniperProjectileSpeed,   // Geschwindigkeit aus der Config
            this->enemy_Damage,                                 // Schaden aus der Config
            game::EnemyConfig::kDrownedSniperProjectileSpriteUp // Das eine Sprite für alle Richtungen
        );

        // 3. Füge das Projektil zum ObjectManager hinzu, damit es im Spiel existiert.
        om_ref_.AddObject(projectile);
    }

    void DrownedSniper::Draw()
    {
        if (game::EnemyConfig::kUseEnemyAnimations)
        {
            // --- Hier kommt später die volle Animationslogik ---
            Vector2 direction = Vector2Normalize({ last_player_position_.x - this->Get_Hitbox_Center().x, last_player_position_.y - this->Get_Hitbox_Center().y });
            // (Code zum Auswählen der richtigen `anim_attack_...` basierend auf `direction`)
            p_current_animation_ = &anim_idle_; // Vorerst immer idle

            if (p_current_animation_)
            {
                Vector2 draw_pos = {
                    this->hitbox.x - (game::EnemyConfig::kDrownedSniperAnimSize.x - this->hitbox.width) / 2,
                    this->hitbox.y - (game::EnemyConfig::kDrownedSniperAnimSize.y - this->hitbox.height) / 2
                };
                p_current_animation_->Draw_Current_Frame(draw_pos, WHITE);
                p_current_animation_->Next_Frame();
            }
        }
        else
        {
            // --- Zeichne das Platzhalter-Sprite ---
            DrawTextureV(this->sprite, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
        }
    }
}