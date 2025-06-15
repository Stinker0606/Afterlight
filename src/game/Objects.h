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

    Rectangle Get_Hitbox() const override{ return hitbox; }
    virtual void Tick(float delta_time) = 0;
    virtual void On_Collision(Collidable* other) = 0;
    virtual void Draw() = 0;
};


#endif //RAYLIBSTARTER_OBJECTS_H
