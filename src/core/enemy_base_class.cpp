//
// Created by $Will on 15.05.2025.
//

#include "enemy_base_class.h"


namespace enemy
{
    //Konstruktor
    enemy_base_class::enemy_base_class(std::string& name, int health, int movement_speed, int damage, int XP)
       : enemy_name(name), enemy_health(health), enemy_movement_speed(movement_speed),
      enemy_damage(damage), enemy_XP(XP) {}

    //Destruktor
    enemy_base_class::~enemy_base_class() = default;

    //Getter
    std::string enemy_base_class::get_name() const
    {
      return enemy_name;
    }
    int enemy_base_class::get_health() const
    {
      return enemy_health;
    }
    int enemy_base_class::get_movement_speed() const
    {
      return enemy_movement_speed;
    }
    int enemy_base_class::get_damage() const
    {
      return enemy_damage;
    }

    //Setter
    void enemy_base_class::set_health(int new_health)
    {
      enemy_health = new_health;
    }

    //Virtuelle Methoden
    void enemy_base_class::enemy_attack() {}
    void enemy_base_class::enemy_take_damage() {}
    void enemy_base_class::enemy_die() {}
}