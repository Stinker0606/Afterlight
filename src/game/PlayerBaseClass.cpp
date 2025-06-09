//
// Created by Kruse on 23/05/2025.
//

#include "PlayerBaseClass.h"
#include "../config.h.in"

Player_Base_Class::Player_Base_Class(int max_health, float movement_speed, int damage, Vector2 start_position)
    : player_Max_Health(max_health), player_Movement_Speed(movement_speed), player_Damage(damage)
{
    player_Health = player_Max_Health; player_Hitbox.x = start_position.x; player_Hitbox.y = start_position.y;
    player_Hitbox.width = game::Config::player_Hitbox_Width; player_Hitbox.height = game::Config::player_Hitbox_Height;
}

void Player_Base_Class::Update(float delta_time)
{
    if (game::Config::enable_Health_Drain)
    {
        this->player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }

    if (this->player_Health < 0)
    {
        this->player_Health = 0;
    }
}

void Player_Base_Class::Take_Damage(int damage_amount)
{
    this->player_Health -= damage_amount;
}
