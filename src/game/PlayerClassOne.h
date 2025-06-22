//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H



#include "PlayerBaseClass.h"

class Player_Class_One : public Player_Base_Class
{
public:
    Player_Class_One(Vector2 start_Position, Collision_Manager* manager);
    ~Player_Class_One();
};


#endif //PLAYERCLASSONE_H
