//
// Created by $Will on 15.05.2025.
//

#ifndef ENEMY_BASE_CLASS_H
#define ENEMY_BASE_CLASS_H

#include <string>

namespace enemy
{
    class enemy_base_class
    {
    protected:
      std::string enemy_name;
      int enemy_health;
      int enemy_movement_speed;
      int enemy_damage;
      int enemy_XP; // Score etc.

    public:

    //Konstruktor
    enemy_base_class(std::string& name, int health, int movement_speed, int damage, int XP);

    //Destruktor
    virtual ~enemy_base_class();

    //Getter
    std::string get_name() const;
    int get_health() const;
    int get_movement_speed() const;
    int get_damage() const;

    //Setter
    void set_health (int new_health);
    //health ist erstmal der einzige Wert den wir während der Laufzeit durch Schaden verändern wollen

    //Methoden welche noch entwickelt werden müssen
    virtual void enemy_attack();
    virtual void enemy_take_damage();
    virtual void enemy_die();
    };
}

#endif //ENEMY_BASE_CLASS_H
