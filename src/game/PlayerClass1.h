//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_WARRIOR_H
#define RAYLIBSTARTER_WARRIOR_H

#include "../config.h.in"
#include "raylib.h"
#include "PlayerBaseClass.h"

class Player_Class_1 : public Player_Base_Class
{
private:
    Texture2D sprite;

public:
    Player_Class_1(Vector2 start_position);
    ~Player_Class_1();

    // Überschreiben der reinen virtuellen Methoden aus der Basisklasse
    void Attack() override;
    void Draw() override;
};


#endif //RAYLIBSTARTER_WARRIOR_H