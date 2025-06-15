//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_OBJECTS_H
#define RAYLIBSTARTER_OBJECTS_H
#include <raylib.h>
#include <string>
#include "Collidable.h"

class Objects : public Collidable
{
protected:
    Vector2 pos;
    Texture2D text;
    Rectangle hitbox;
    std::string name;

    Objects()=default;
    Objects(Vector2,std::string);
public:
    virtual void Draw();

    Rectangle Get_Hitbox() const override
    {
        return hitbox;
    }
    CollisionType Get_Collision_Type() const override = 0;
    void On_Collision(Collidable* other) override = 0;
};


#endif //RAYLIBSTARTER_OBJECTS_H
