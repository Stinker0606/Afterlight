//
// Created by $Will on 11.06.2025.
//

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include <memory>
#include <algorithm>
#include <set>
#include "Quadtree.h"
#include "../game/Collidable.h"
#include "raylib.h"



class Collision_Manager
{
private:
    std::vector<std::shared_ptr<Collidable>>& collidables;
    std::unique_ptr<Quadtree> quadtree;

public:
    Collision_Manager(Rectangle world_Bounds,std::vector<std::shared_ptr<Collidable>>& collidables);

    /*void Regist_Object(std::shared_ptr<Collidable> object);
    void Unregist_Object(std::shared_ptr<Collidable> object);*/
    void Check_Collisions();
};

#endif //COLLISIONMANAGER_H
