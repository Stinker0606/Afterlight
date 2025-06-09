//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_OBJECTS_H
#define RAYLIBSTARTER_OBJECTS_H
#include <raylib.h>
#include <string>

class Objects
{
protected:
    Vector2 pos;
    Texture2D text;
    Rectangle hitbox;
    std::string name;
    Objects()=default;
    Objects(Vector2,std::string);
public:

    virtual void draw();
};


#endif //RAYLIBSTARTER_OBJECTS_H
