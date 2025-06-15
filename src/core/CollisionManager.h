//
// Created by $Will on 11.06.2025.
//

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <memory>
#include <algorithm>
#include <set>

#include "raylib.h"

class Collidable;
class Quadtree;
class Collision_Manager
{
private:
    std::vector<Collidable*> collidables;
    std::unique_ptr<Quadtree> quadtree;

public:
    Collision_Manager(Rectangle world_bounds);

    void Regist_Object(Collidable* object);
    void Unregist_Object(Collidable* object);
    void Check_Collisions();
};

#endif //COLLISIONMANAGER_H
