//
// Created by Kruse on 23/05/2025.
//

#include "PlayerBaseClass.h"
#include "../config.h.in"
#include "CollisionManager.h"

Player_Base_Class::Player_Base_Class(int max_health, float movement_speed, int damage, Vector2 start_position,
                                     Collision_Manager* manager_ptr)
    : player_Max_Health(max_health), player_Movement_Speed(movement_speed), player_Damage(damage)
{
    player_Health = player_Max_Health; player_Hitbox.x = start_position.x; player_Hitbox.y = start_position.y;
    player_Hitbox.width = game::Config::player_Hitbox_Width; player_Hitbox.height = game::Config::player_Hitbox_Height;
}

void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = player_Hitbox.x;
    previous_Position.y = player_Hitbox.y;
}

void Player_Base_Class::Stop_Movement()
{
    player_Hitbox.x = previous_Position.x;
    player_Hitbox.y = previous_Position.y;
}

void Player_Base_Class::Update_Facing_Direction()
{
    bool up = IsKeyDown(game::Config::key_Up);
    bool down = IsKeyDown(game::Config::key_Down);
    bool left = IsKeyDown(game::Config::key_Left);
    bool right = IsKeyDown(game::Config::key_Right);

    if ((up && down) || (left && right)) return;

    if (up && right) facing_Direction = Facing_Direction::UP_RIGHT;
    else if (up && left) facing_Direction = Facing_Direction::UP_LEFT;
    else if (down && right) facing_Direction = Facing_Direction::DOWN_RIGHT;
    else if (down && left) facing_Direction = Facing_Direction::DOWN_LEFT;
    else if (up) facing_Direction = Facing_Direction::UP;
    else if (down) facing_Direction = Facing_Direction::DOWN;
    else if (left) facing_Direction = Facing_Direction::LEFT;
    else if (right) facing_Direction = Facing_Direction::RIGHT;
}

void Player_Base_Class::Player_Input()
{
    if (IsKeyPressed(game::Config::key_Melee_Attack) && melee_Cooldown <= 0)
    {
        Melee_Attack();
    }

    if (IsKeyPressed(game::Config::key_Ranged_Attack) && ranged_Cooldown <= 0)
    {
        Ranged_Attack();
    }

    if (IsKeyPressed(game::Config::key_Use_Item) && inventory_Is_Full)
    {
        Use_Item();
    }
}

void Player_Base_Class::Tick(float delta_time)
{
    if (game::Config::enable_Health_Drain)
    {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }
    Update_Previous_Position();

	Vector2 move_Direction = {0.0f, 0.0f};
    if (IsKeyDown(game::Config::key_Up))    move_Direction.y -= 1.0f;
    if (IsKeyDown(game::Config::key_Down))  move_Direction.y += 1.0f;
    if (IsKeyDown(game::Config::key_Left))  move_Direction.x -= 1.0f;
    if (IsKeyDown(game::Config::key_Right)) move_Direction.x += 1.0f;

    if (move_Direction.x != 0.0f || move_Direction.y != 0.0f)
    {
        move_Direction = Vector2Normalize(move_Direction);
    }

    player_Hitbox.x += move_Direction.x * player_Movement_Speed * delta_time;
    player_Hitbox.y += move_Direction.y * player_Movement_Speed * delta_time;

    Update_Facing_Direction();

    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (ranged_Cooldown > 0) ranged_Cooldown -= delta_time;
}

void Player_Base_Class::On_Collision(Collidable* other)
{
	Collision_Type otherType = other->Get_Collision_Type();

    if (otherType == Collision_Type::WALL ||
        otherType == Collision_Type::ENEMY_SPAWNER ||
        otherType == Collision_Type::ENEMY)
    {
		Rectangle wall_Hitbox = other->Get_Hitbox();
        if (CheckCollisionRecs({player_Hitbox.x, previous_Position.y, player_Hitbox.width, player_Hitbox.height}, wall_Hitbox))
        {
            player_Hitbox.y = previous_Position.y;
        }
        if (CheckCollisionRecs({previous_Position.x, player_Hitbox.y, player_Hitbox.width, player_Hitbox.height}, wall_Hitbox))
        {
            player_Hitbox.x = previous_Position.x;
		}
	}
}
void Player_Base_Class::Draw()
{

}

void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown;
}
void Player_Base_Class::Ranged_Attack()
{
	ranged_Cooldown;
}