//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS_H


#include "base_class.h"

// Inherits from base class and adds player only logic (like the HP drain from Gauntlet)

class player_base_class : public base_class {
protected:
    float hp_timer = 0.0f;

public:
    void UPDATE(float delta_time) override;
};


#endif //RAYLIBSTARTER_PLAYER_CLASS_H
