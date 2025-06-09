//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS

#include "raylib.h"
#include <string>

class Player_Base_Class
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

	public:
    	// Konstruktor
    	Player_Base_Class(int max_health, float movement_speed, int damage, Vector2 start_position);

    	// Destruktor
    	virtual ~Player_Base_Class() = default;

    	virtual void Update(float delta_time);
    	void Take_Damage(int damage_amount);
    	virtual void Attack() = 0;
    	virtual void Draw() = 0;

    	float Get_Health() const { return player_Health; }
    	int Get_Damage() const { return player_Damage; }
    	Rectangle Get_Hitbox() const { return player_Hitbox; }
};

#endif //RAYLIBSTARTER_PLAYER_CLASS_H
