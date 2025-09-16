//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>
#include "Collidable.h"

enum class EnemyState { IDLE, CHASING, ATTACKING, DYING };

class Object_Manager;

namespace enemy
{
    //class Collision_Manager;
    class Enemy_Base_Class : public Collidable
    {
    protected:

        std::string enemy_Name;
        int enemy_Health;
        int enemy_Movement_Speed;
        int enemy_Damage;
        const float attack_Cooldown_Duration;
        float attack_Cooldown_Timer;
        int enemy_Value;
        bool is_Moving;
        float move_sound_timer_;
        Texture2D sprite;
        const char* projectile_sprite_path;
        void Pathfinding(Vector2 target_Position, float delta_Time, float attack_range);
        EnemyState current_state;
        float attack_animation_timer;
        Object_Manager& om_ref_;
        bool is_animation_active_;
        Color tint_color;
        float hit_feedback_timer;
        float hit_feedback_total_time;
        int hit_feedback_blinks_left;
        bool hit_feedback_on;
    public:
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
           const char* sprite_path, const char* projectile_sprite_path,Vector2 start_position, int width, int height,
           float cooldown_duration, Object_Manager& om);

        virtual ~Enemy_Base_Class();

        // --- KI-Schnittstelle ---
        virtual void Update_AI(float delta_time, Vector2 player_position) = 0;

        // --- Angriffs-Schnittstellen ---
        virtual void Melee_Attack() = 0;
        virtual void Range_Attack() = 0;
        void Set_Animation_Active(bool is_active);

        // --- Öffentliche Methoden ---
        virtual void Take_Damage(int damage_amount);
        virtual void PlayHitSound();
        virtual void PlayDeathSound();
        virtual void PlayMoveSound();
        int Get_Health() const { return enemy_Health; }
        int Get_Damage() const { return enemy_Damage; }
        int Get_Movement_Speed(){return enemy_Movement_Speed;};

        // --- Von Collidable geerbte Methoden ---
        Collision_Type Get_Collision_Type() const override { return Collision_Type::ENEMY; }
        void Tick(float delta_time) override;
        void On_Collision(std::shared_ptr<Collidable> other) override;
        void Draw() override;
    };
}
#endif

