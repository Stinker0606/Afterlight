//
// Created by Kruse on 23/05/2025.
//
#pragma once


#include "raylib.h"
#include "Collidable.h"
#include "CollisionManager.h"
#include "../Config.h.in"
#include "raymath.h"
#include "Object_Manager.h"
#include "FacingDirection.h"


namespace game {
	class Player_Projectile;
}

class Collision_Manager;

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

	Texture2D maintex= LoadTexture("PLACEHOLDER");

    float projectile_Speed;
    std::vector<std::shared_ptr<game::Player_Projectile>> sp_projectiles;
    Object_Manager& om;

	// Ein Enum für klare Spieler-Zustände
	enum class PlayerState { IDLE, MOVING, ATTACKING_RANGED, ATTACKING_MELEE, PUSHING, DYING };
	PlayerState player_state;

	// Ein Timer der steuert wann nach der Animation der Schuss ausgelöst wird
	float attack_animation_timer;

public:
	// Konstruktor
	Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position, Object_Manager& om);

	// Destruktor
	~Player_Base_Class() override;
	void Player_Input();
	void Tick(float delta_time) override;
	void On_Collision(std::shared_ptr<Collidable> other) override;
	virtual void Draw() override;

	void Update_Previous_Position();
	void Update_Facing_Direction();

	void Melee_Attack();
	virtual void Ranged_Attack();
	void Use_Item();

    Collision_Type Get_Collision_Type() const override;
    Vector2 Get_Player_Pos();
    Vector2 Get_Player_Center();

    void Take_Damage(int damage);

};

