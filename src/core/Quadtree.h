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

    std::vector<std::shared_ptr<Collidable>> objects;

    Rectangle bounds;

    std::unique_ptr<Quadtree> nodes[4];

    void Split();

    int Get_Index(std::shared_ptr<Collidable> object);

public:
    Quadtree(int pLevel, Rectangle pBounds);

    void Clear();

    void Insert(std::shared_ptr<Collidable> object);

    std::vector<std::shared_ptr<Collidable>> Retrieve(std::shared_ptr<Collidable> object);
};

#endif //QUADTREE_H
