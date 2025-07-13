//
// Created by $Will on 04.07.2025.
//

#ifndef COLLISIONRESPONSE_H
#define COLLISIONRESPONSE_H



#include "Collidable.h"

class CollisionResponse
{
public:

    static void Resolve_Overlap(Collidable* obj_A, const Collidable* obj_B);

    static void Apply_Damage(Collidable* target, int damage);

    static void Mark_For_Destruction(Collidable* obj_To_Destroy);
};


#endif //COLLISIONRESPONSE_H
