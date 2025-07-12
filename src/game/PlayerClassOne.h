//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H



#include "PlayerBaseClass.h"
#include "RepeatAnimation.h"

class Player_Class_One : public Player_Base_Class
{
private:
    Vector2 size{32,48};


public:
    RepeatAnimation test_animation{size,"assets/graphics/MC_Walkcycle_Front.png",10,10};
    Player_Class_One(Vector2 start_Position);
    ~Player_Class_One();
    void Draw() override;

};


#endif //PLAYERCLASSONE_H
