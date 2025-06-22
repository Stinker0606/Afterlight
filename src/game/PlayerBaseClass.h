//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS

#include "raylib.h"
#include "raymath.h"
#include <string>
#include "Collidable.h"
#include "SpriteAnimated.h"

class Collision_Manager;

enum class Facing_Direction {UP, DOWN, LEFT, RIGHT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};

class Player_Base_Class : public Collidable
{
protected:
	// Kern-Attribute
	float player_Health;
	int player_Max_Health;
	float player_Movement_Speed;
	int player_Damage;

	// Player Länge und Breite sind in der Config. Die Position muss man noch mit dem passenden Vektor und dem
	// Spieler Spawner verbinden
	Rectangle player_Hitbox;
	Vector2 previous_Position;
	Collision_Manager* manager_Ptr;

	float melee_Cooldown;
	float ranged_Cooldown;
	bool inventory_Is_Full;
	Facing_Direction facing_Direction;

public:
	// Konstruktor
	Player_Base_Class(int max_Health, float movement_Speed, int damage,
						Vector2 start_Position, Collision_Manager* manager = nullptr);
	// Destruktor
	~Player_Base_Class();
	void Player_Input();
	void Tick(float delta_time);
	void On_Collision(Collidable* other) override;
	virtual void Draw() = 0;

	void Update_Previous_Position();
	void Stop_Movement();

protected:
	void Melee_Attack();
	void Ranged_Attack();
	void Use_Item();

	void Update_Facing_Direction();
};

#endif //RAYLIBSTARTER_PLAYER_CLASS_H
