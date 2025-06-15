//
// Created by $Will on 12.06.2025.
//

#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <memory>
#include "raylib.h"

class Collidable;
class Quadtree
{
private:
    static const int max_Objects = 6;
    static const int max_Levels = 3;

    int level;

    std::vector<Collidable*> objects;

    Rectangle bounds;

    std::unique_ptr<Quadtree> nodes[4];

    void Split();

    int Get_Index(Collidable* object);

public:
    Quadtree(int pLevel, Rectangle pBounds);

    void Clear();

    void Insert(Collidable* object);

    std::vector<Collidable*> Retrieve(Collidable* object);
};

#endif //QUADTREE_H
