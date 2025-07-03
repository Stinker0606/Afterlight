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
        int enemy_Value;
        Texture2D sprite;
        Collision_Manager* manager_ptr;

    public:
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
            const char* sprite_path, Vector2 start_position, int width, int height, Collision_Manager* manager);

        virtual ~Enemy_Base_Class();

        void Take_Damage(int damage_amount);
        virtual void Range_Attack();
        virtual void Melee_Attack();
        virtual void Pathfinding()=0;

        int Get_Health() const { return enemy_Health; }
        int Get_Damage() const { return enemy_Damage; }

        Collision_Type Get_Collision_Type() const override { return Collision_Type::ENEMY; }

        void Tick(float delta_time);
        void On_Collision(Collidable* other) override;
        void Draw();
    };
}
#endif

