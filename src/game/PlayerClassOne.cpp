//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position, Collision_Manager* manager)

    : Player_Base_Class(game::Config::player_Max_Health, game::Config::player_Movement_Speed,
        game::Config::player_Damage, game::Config::player_Sprite_Path, start_Position, manager) {}

Player_Class_One::~Player_Class_One(){}