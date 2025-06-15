//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS

#include "raylib.h"
#include <string>
#include "Collidable.h"

class Collision_Manager;
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
		Vector2 previous_position;
        Collision_Manager* manager_ptr;

	public:
    	// Konstruktor
    	Player_Base_Class(int max_health, float movement_speed, int damage,
                            Vector2 start_position, Collision_Manager* manager);
    	// Destruktor
    	virtual ~Player_Base_Class() = default;


    	void Take_Damage(int damage_amount);
    	virtual void Attack() = 0;


    	float Get_Health() const { return player_Health; }
    	int Get_Damage() const { return player_Damage; }
        Rectangle Get_Hitbox() const override { return player_Hitbox; }
    	CollisionType Get_Collision_Type() const override { return CollisionType::PLAYER; }
		void Update_Previous_Position();
		void Stop_Movement();

        void Player_Input();
		void Tick(float delta_time);
		void On_Collision(Collidable* other) override;
		void Draw();
};

#endif //RAYLIBSTARTER_PLAYER_CLASS_H
