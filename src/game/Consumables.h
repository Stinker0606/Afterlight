//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_CONSUMABLES_H
#define RAYLIBSTARTER_CONSUMABLES_H


#include "Collidable.h"

class Collision_Manager;
class Consumables : public Collidable
{
protected:

    Texture2D texture;
    std::string name;

public:
    Consumables(Vector2 position, std::string name);
    ~Consumables();


    Collision_Type Get_Collision_Type() const override;

    void Tick(float delta_time) override;
    void On_Collision(Collidable* other) override;
    void Draw() override;
};


#endif //RAYLIBSTA