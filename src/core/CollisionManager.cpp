//
// Created by $Will on 11.06.2025.
//

#include "CollisionManager.h"
#include "Quadtree.h"
#include "../game/Collidable.h"

Collision_Manager::Collision_Manager(Rectangle world_bounds)
{
    quadtree = std::make_unique<Quadtree>(0, world_bounds);
}

void Collision_Manager::Regist_Object(Collidable* object)
{
    collidables.push_back(object);
}

void Collision_Manager::Unregist_Object(Collidable* object)
{
    for (auto it = collidables.begin(); it != collidables.end(); ++it)
    {
        if (*it == object)
        {
            collidables.erase(it);
            break;
        }
    }
}

void Collision_Manager::Check_Collisions()
{
    quadtree->Clear();
    for (Collidable* obj : collidables)
    {
        quadtree->Insert(obj);
    }

    std::set<std::pair<Collidable*, Collidable*>> processed_pairs;

    for (Collidable* objA : collidables)
    {
        std::vector<Collidable*> candidates = quadtree->Retrieve(objA);

        for (Collidable* objB : candidates)
        {
            if (objA == objB) continue;
            auto pair = std::minmax(objA, objB);
            if (processed_pairs.count(pair)) continue;

            if (CheckCollisionRecs(objA->Get_Hitbox(), objB->Get_Hitbox()))
            {
                objA->On_Collision(objB);
                objB->On_Collision(objA);
            }

            processed_pairs.insert(pair);
        }
    }
}
