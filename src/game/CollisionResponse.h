//
// Created by $Will on 04.07.2025.
//

#ifndef COLLISIONRESPONSE_H
#define COLLISIONRESPONSE_H



#include "Collidable.h"
#include <memory>

class CollisionResponse
{
public:

    static void Resolve_Overlap(std::shared_ptr<Collidable> obj_A, const std::shared_ptr<Collidable> obj_B);

    static void Apply_Damage(std::shared_ptr<Collidable> target, int damage);

    static void Mark_For_Destruction(std::shared_ptr<Collidable> obj_To_Destroy);
};


#endif //COLLISIONRESPONSE_H
