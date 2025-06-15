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
    void Draw() override;

    CollisionType Get_Collision_Type() const override;
    void On_Collision(Collidable* other) override;
};


#endif //RAYLIBSTARTER_CONSUMABLES_H
