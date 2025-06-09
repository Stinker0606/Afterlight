//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_CONSUMABLES_H
#define RAYLIBSTARTER_CONSUMABLES_H


#include "Objects.h"
#include "config.h"

class Consumables: public Objects
{
public:
    Consumables(Vector2,std::string);
    void draw() override;
};


#endif //RAYLIBSTARTER_CONSUMABLES_H
