//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"

Player_Class_One::Player_Class_One(Vector2 start_Position, Collision_Manager* manager)

: Player_Base_Class( game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
    game::Config::player_Class_One_Damage, game::Config::player_Class_One_Sprite_Path,
    game::Config::player_Class_One_Frame_Width,  game::Config::player_Class_One_Frame_Height,
    game::Config::player_Class_One_Walk_Anim_Frames, game::Config::player_Class_One_Anim_Speed,
    start_Position, manager) {}

Player_Class_One::~Player_Class_One(){}