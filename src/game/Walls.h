//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_WALLS_H
#define RAYLIBSTARTER_WALLS_H


#include "Collidable.h"

class Collision_Manager;
class Walls : public Collidable
{
protected:

    Collision_Manager* manager_ptr;

public:
    Walls(Vector2 position, Vector2 size, Collision_Manager* cm);
    ~Walls();


    Collision_Type Get_Collision_Type() const override;


    void Tick(float delta_time);
    void On_Collision(Collidable* other) override;
    void Draw();
    Rectangle Get_Hitbox();
};

#endif //RAYLIBSTARTER_WALLS_H
