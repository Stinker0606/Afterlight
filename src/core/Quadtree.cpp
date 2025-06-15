//
// Created by $Will on 12.06.2025.
//

#include "Quadtree.h"
#include "../game/Collidable.h"

Quadtree::Quadtree(int pLevel, Rectangle pBounds)
{
    level = pLevel;
    bounds = pBounds;
}

void Quadtree::Clear()
{
    objects.clear();

    for (int i = 0; i < 4; i++)
    {
        if (nodes[i])
        {
            nodes[i] ->Clear();
            nodes[i].reset();
        }
    }
}

void Quadtree::Split()
{
    float subWidth = bounds.width / 2;
    float subHeight = bounds.height / 2;
    float x = bounds.x;
    float y = bounds.y;

    nodes[0] = std::make_unique<Quadtree>(level + 1, Rectangle{ x, y, subWidth, subHeight });
    nodes[1] = std::make_unique<Quadtree>(level + 1, Rectangle{ x + subWidth, y, subWidth, subHeight });
    nodes[2] = std::make_unique<Quadtree>(level + 1, Rectangle{ x, y + subHeight, subWidth, subHeight });
    nodes[3] = std::make_unique<Quadtree>(level + 1, Rectangle{ x + subWidth, y + subHeight, subWidth, subHeight });
}

int Quadtree::Get_Index(Collidable* object)
{
    int index = -1;

    Rectangle objectBounds = object->Get_Hitbox();

    float verticalMidpoint = this->bounds.x + (this->bounds.width / 2.0f);
    float horizontalMidpoint = this->bounds.y + (this->bounds.height / 2.0f);

    bool fitsInTop = (objectBounds.y < horizontalMidpoint && objectBounds.y + objectBounds.height < horizontalMidpoint);
    bool fitsInBottom = (objectBounds.y > horizontalMidpoint);

    if (objectBounds.x < verticalMidpoint && objectBounds.x + objectBounds.width < verticalMidpoint)
    {
        if (fitsInTop)
        {
            index = 0;
        }
        else if (fitsInBottom)
        {
            index = 2;
        }
    }

    else if (objectBounds.x > verticalMidpoint)
    {
        if (fitsInTop)
        {
            index = 1;
        }
        else if (fitsInBottom)
        {
            index = 3;
        }
    }

    return index;
}

void Quadtree::Insert(Collidable* object)
{
    if (nodes[0])
    {
        int index = Get_Index(object);
        if (index != -1)
        {
            nodes[index]->Insert(object);
            return;
        }
    }

    objects.push_back(object);

    if (objects.size() > max_Objects && level < max_Levels)
    {
        if (!nodes[0])
        {
            Split();
        }

        int i = 0;
        while (i <objects.size())
        {
            int index = Get_Index(objects[i]);

            if (index != -1)
            {
                nodes[index]->Insert(objects[i]);
                objects.erase(objects.begin() + i);
            }
            else
            {
                i++;
            }
        }
    }
}

std::vector<Collidable*> Quadtree::Retrieve(Collidable* object)
{
    std::vector<Collidable*> returnObjects;

    int index = this->Get_Index(object);

    if (nodes[0] && index != -1)
    {
        returnObjects = nodes[index]->Retrieve(object);
    }

    returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

    return returnObjects;
}