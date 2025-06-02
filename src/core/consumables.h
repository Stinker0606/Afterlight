//
// Created by Kruse on 02/06/2025.
//

#ifndef RAYLIBSTARTER_CONSUMABLES_H
#define RAYLIBSTARTER_CONSUMABLES_H


#include "objects.h"
#include "config.h"

class consumables: public objects {
public:
    consumables(Vector2,std::string);
    void draw() override;
};


#endif //RAYLIBSTARTER_CONSUMABLES_H
