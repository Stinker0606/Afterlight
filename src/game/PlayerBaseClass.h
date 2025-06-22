//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS

#include "raylib.h"
#include "raymath.h"
#include <string>
#include "Collidable.h"
#include "Actor.h"
#include "../config.h.in"
#include "CollisionManager.h"
#include "SpriteAnimated.h"
#include "Texture2d.h"
#include <memory>

class Collision_Manager;

enum class Facing_Direction {UP, DOWN, LEFT, RIGHT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};

class Player_Base_Class : public game::core::Actor, public Collidable
{
protected:
	// Kern-Attribute
	float player_Health;
	int player_Max_Health;
	float player_Movement_Speed;
	int player_Damage;

	Rectangle player_Hitbox;
	Vector2 previous_Position;
	Collision_Manager* manager_Ptr;

	float melee_Cooldown;
	float ranged_Cooldown;
	bool inventory_Is_Full;
	Facing_Direction facing_Direction;
	bool is_Moving;

public:
	// Konstruktor
	Player_Base_Class(int max_Health, float movement_Speed, int damage, const char* sprite_Path, int frame_Width,
		int frame_Height, int anim_Frames, int anim_Speed, Vector2 start_Position, Collision_Manager* manager);

	// Destruktor
	~Player_Base_Class();
	void Player_Input();
	void Tick(float delta_time);
	void On_Collision(Collidable* other) override;

	void Update_Previous_Position();
	void Update_Facing_Direction();
	void Stop_Movement();

	void Melee_Attack();
	void Ranged_Attack();
	void Use_Item();

    Rectangle Get_Hitbox() const override;
    Collision_Type Get_Collision_Type() const override;
};

#endif //RAYLIBSTARTER_PLAYER_CLASS_H
