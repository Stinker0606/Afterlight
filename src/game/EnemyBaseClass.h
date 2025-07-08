//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>
#include "Collidable.h"

namespace enemy
{
    class Collision_Manager;
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
        Texture2D sprite;
        const char* projectile_sprite_path;

    public:
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
            const char* sprite_path, const char* projectile_sprite_path,Vector2 start_position, int width, int height,
            float cooldown_duration);

        virtual ~Enemy_Base_Class();

        void Take_Damage(int damage_amount);
        virtual void Range_Attack();
        virtual void Melee_Attack();
        virtual void Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time);

        int Get_Health() const { return enemy_Health; }
        int Get_Damage() const { return enemy_Damage; }
        int Get_Movement_Speed(){return enemy_Movement_Speed;};

        Collision_Type Get_Collision_Type() const override { return Collision_Type::ENEMY; }

        void Tick(float delta_time);
        void On_Collision(Collidable* other) override;
        void Draw();
    };
}
#endif

