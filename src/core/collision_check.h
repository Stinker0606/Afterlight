//
// Created by $Will on 01.06.2025.
//

#ifndef COLLISSION_CHECK_H
#define COLLISSION_CHECK_H
#include "raylib.h"

namespace game::core
{
    //Grundlegender collision_check aus der raylib für 2 Rechtecke
    bool check_collision(Rectangle rec1, Rectangle rec2);

}
#endif //COLLISSION_CHECK_H
