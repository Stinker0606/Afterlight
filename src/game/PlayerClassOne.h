//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H



#include "PlayerBaseClass.h"
#include "Animations.h"

class Player_Class_One : public Player_Base_Class
{
private:
    Vector2 size{80,80};


public:
    Animations test_animation{size,"assets/graphics/anim_sprite.png",9,3};
    Player_Class_One(Vector2 start_Position);
    ~Player_Class_One();
    void Draw();

};


#endif //PLAYERCLASSONE_H
