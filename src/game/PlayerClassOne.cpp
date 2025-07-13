//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage, start_Position){}



Player_Class_One::~Player_Class_One() {}

void Player_Class_One::Draw() {
    test_animation.Draw_Current_Frame(this->player_Pos);
    test_animation.Next_Frame();
}