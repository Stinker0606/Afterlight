//
// Created by Kruse on 23/05/2025.
//

#include "Enemy_Base_Class.h"


namespace enemy
{
    //Konstruktor
    Enemy_Base_Class::Enemy_Base_Class(std::string& name, int health, int movement_speed, int damage, int value)
            : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed),
              enemy_Damage(damage), enemy_Value(value) {}

    //Destruktor
    Enemy_Base_Class::~Enemy_Base_Class() = default;

    //Getter
    std::string Enemy_Base_Class::get_Name() const
    {
        return enemy_Name;
    }
    int Enemy_Base_Class::get_Health() const
    {
        return enemy_Health;
    }
    int Enemy_Base_Class::get_Movement_Speed() const
    {
        return enemy_Movement_Speed;
    }
    int Enemy_Base_Class::get_Damage() const
    {
        return enemy_Damage;
    }
    int Enemy_Base_Class::get_Value() const
    {
        return enemy_Value;
    }

    //Setter
    void Enemy_Base_Class::set_Health(int new_Health)
    {
        enemy_Health = new_Health;
    }

    //Virtuelle Methoden
    void Enemy_Base_Class::Enemy_Attack() {}
    void Enemy_Base_Class::Enemy_Take_Damage() {}
    void Enemy_Base_Class::Enemy_Draw() {}
    void Enemy_Base_Class::Enemy_Pathfinding() {}
}
