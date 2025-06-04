//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_WARRIOR_H
#define RAYLIBSTARTER_WARRIOR_H


#include "base_class.h"
#include "../config.h.in"
#include "raylib.h"

class warrior : public base_class
{
private:
    Texture2D sprite;

public:
    warrior();
    ~warrior() override;

    void ATTACK() override;
    void UPDATE(float delta_time) override;
    void DRAW() const;
};


#endif //RAYLIBSTARTER_WARRIOR_H