//
// Created by Kruse on 23/05/2025.
//
#pragma once


#include "raylib.h"
#include "Collidable.h"
#include "CollisionManager.h"
#include "../Config.h.in"
#include "raymath.h"
#include "PlayerProjectile.h"

class Collision_Manager;

enum Facing_Direction {UP, DOWN, LEFT, RIGHT, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};

class Player_Base_Class : public Collidable
{
protected:

	float player_Health;
	int player_Max_Health;
	float player_Movement_Speed;
	int player_Damage;

    Vector2 player_Pos;
	Vector2 previous_Position;
	Collision_Manager* manager_Ptr;

	float melee_Cooldown;
	float ranged_Cooldown;
	bool inventory_Is_Full;
	Facing_Direction facing_Direction;
	bool is_Moving;


    Texture2D maintex= LoadTexture("assets/graphics/ball.png");

    float projectile_Speed;
    std::vector<std::unique_ptr<game::Player_Projectile>> sp_projectiles;

public:
	// Konstruktor
	Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position);

	// Destruktor
	~Player_Base_Class() override;
	void Player_Input();
	void Tick(float delta_time);
	void On_Collision(Collidable* other) override;
	virtual void Draw();

	void Update_Previous_Position();
	void Update_Facing_Direction();

	void Melee_Attack();
	void Ranged_Attack();
	void Use_Item();

    Collision_Type Get_Collision_Type() const override;
    Vector2 Get_Player_Pos();

    void Take_Damage(int damage);
};

