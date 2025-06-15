//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_CONSUMABLES_H
#define RAYLIBSTARTER_CONSUMABLES_H


#include "Objects.h"

class Consumables: public Objects
{
public:
    Consumables(Vector2,std::string);

    CollisionType Get_Collision_Type() const override;

    void Tick(float delta_time);
    void On_Collision(Collidable* other) override;
    void Draw() override;
};


#endif //RAYLIBSTA