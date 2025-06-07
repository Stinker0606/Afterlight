//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYER_CLASS_H
#define RAYLIBSTARTER_PLAYER_CLASS

#include <string>

// Inherits from base class and adds player only logic (like the HP drain from Gauntlet)

class Player_Base_Class
{
	protected:

    int hp_Timer = 0.0f; 	//If your Game has an hp_timer
    int hp_Drain_Speed;		//If your Game has an hp_timer

    std::string Player_Class_Name;
    int player_Health;
    int player_Movement_Speed;
    int player_Damage;

    //float player_Value; // Dieses Attribut kann später genutzt werden um Score/ Levelprogress oder ähnliches
  						  // für den Spieler verwalten.
	public:
    	// Konstruktor
    	Player_Base_Class(std::string& name, int health, int movement_Speed, int damage, int value);

        //Destruktor
   		virtual ~Player_Base_Class() = default;

        //Getter & Setter
    	int Get_Health() const { return player_Health; }
    	int Get_Speed() const { return player_Movement_Speed; }
    	int Get_Damage() const { return player_Damage; }
    	int Set_Health(float health) { player_Health = health; }


    void Update(float delta_time);
	virtual void Attack() = 0;
};

#endif //RAYLIBSTARTER_PLAYER_CLASS_H
