//
// Created by $Will on 11.06.2025.
//

#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "raylib.h"
#include <string>

enum class Collision_Type
{
    PLAYER,
    ENEMY,
    WALL,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE,
    CONSUMABLE,
    PLAYER_MELEE_HITBOX,
    ENEMY_SPAWNER
};

class Collidable
{
protected:
    Rectangle hitbox;
public:
    virtual ~Collidable() = default;

    Rectangle Get_Hitbox() const{return this->hitbox;};
    virtual Collision_Type Get_Collision_Type() const = 0;
    virtual void On_Collision(Collidable* other) = 0;
};
#endif //COLLIDABLE_H
