//
// Created by Kruse on 23/05/2025.
//

#include "Player_Base_Class.h"
#include "../config.h.in"


void Player_Base_Class::Update(float delta_time)
{

    hp_Timer += delta_time;
    if (hp_Timer >= 1.0f)
    {
        hp_Drain_Speed -= game::Config::hp_Drain_Speed;
        hp_Timer = 0.0f; // Reset timer
    }

}

