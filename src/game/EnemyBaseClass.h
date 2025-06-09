//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_ENEMY_BASE_CLASS_H
#define RAYLIBSTARTER_ENEMY_BASE_CLASS_H

#include "raylib.h"
#include <string>

namespace enemy
{
    class Enemy_Base_Class
    {
    protected:

        std::string enemy_Name;
        int enemy_Health;
        int enemy_Movement_Speed;
        int enemy_Damage;
        int enemy_Value; // Score etc.
        Rectangle enemy_Hitbox;
        Texture2D sprite;

    public:
        //Konstruktor
        Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
            const char* sprite_path, Vector2 start_position, int width, int height);

        //Destruktor
        virtual ~Enemy_Base_Class();

        // Diese Methoden sind erstmal ein Standard. Virtuell heißt sie können überschrieben werden aber ohne
        // = 0 müssen sie nicht unbedingt überschrieben werden und können als Standard genutzt werden.
        void Take_Damage(int damage_amount);
        virtual void Attack();
        virtual void Draw();
        virtual void Pathfinding()=0;
        virtual void Update();

        int get_Health() const { return enemy_Health; }
        int get_Damage() const { return enemy_Damage; }
        Rectangle get_Hitbox() const { return enemy_Hitbox; }
    };
}


#endif //RAYLIBSTARTER_ENEMY_BASE_CLASS_H
