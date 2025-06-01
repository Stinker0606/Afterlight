//
// Created by Kruse on 23/05/2025.
//
#include "raylib.h"

namespace game::core
{

    bool check_collision(Rectangle rec1, Rectangle rec2)
    {
        return CheckCollisionRecs(rec1, rec2);
    }

}

