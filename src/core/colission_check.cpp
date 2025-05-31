//
// Created by Kruse on 23/05/2025.
//
#include "raylib.h"

bool check_collision(Rectangle rec1, Rectangle rec2)
{
    bool collision = false;

    if (CheckCollisionRecs(rec1, rec2))
    {
        collision = true;
    }

    return collision;
}

