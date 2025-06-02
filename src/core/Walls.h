//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_WALLS_H
#define RAYLIBSTARTER_WALLS_H


#include "objects.h"

class Walls: public objects{
protected:
    Vector2 size{};
public:
    Walls(Vector2 position,Vector2 size);
    void draw() override;
};


#endif //RAYLIBSTARTER_WALLS_H
