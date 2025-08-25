//
// Created by Kruse on 23/05/2025.
//

#include <valarray>
#include "EnemyBaseClass.h"
#include <iostream>
#include "Store.h"
#include "AssetManager.h"
#include "CollisionManager.h"
#include "CollisionResponse.h"
#include "PlayerBaseClass.h"
#include <valarray>

namespace enemy
{
Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
    const char* sprite_path, const char* projectile_sprite_path,Vector2 start_position, int width, int height, float cooldown_Duration, Object_Manager& om)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed), enemy_Damage(damage),
      enemy_Value(value),attack_Cooldown_Duration(cooldown_Duration), attack_Cooldown_Timer(0.0f), is_Moving(false), om_ref_(om)
    {
    hitbox = {start_position.x, start_position.y, (float)width, (float)height};
    sprite = AssetManager::GetInstance().Load(sprite_path);
    }

Enemy_Base_Class::~Enemy_Base_Class()
{
    //UnloadTexture(sprite);
}

    void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    // Verhindere, dass bereits tote Gegner mehrfach Punkte geben
    if (this->enemy_Health <= 0) return;

    enemy_Health -= damage_amount;

    if (this->enemy_Health <= 0)
    {
        // Logik für die Punktevergabe
        if (game::core::Store::player) {
            game::core::Store::player->Add_Score(this->enemy_Value);
        }

        this->Mark_For_Destruction();
    }
}


void Enemy_Base_Class::Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time)
{
    // Berechnet den Richtungs-Vektor vom Gegner zum Ziel.
    float delta_Vector_X = target_Position_X - this->hitbox.x;
    float delta_Vector_Y = target_Position_Y - this->hitbox.y;

    // Berechnet die exakte Distanz zum Ziel.
    float distance_To_Target = std::sqrt(delta_Vector_X * delta_Vector_X + delta_Vector_Y * delta_Vector_Y);

    // Wir holen uns die Angriffsreichweite.
    float attack_Range = 16.0; // 0,5 Tiles

    // Sicherheitscheck, um eine Division durch Null (Fehler den ich bei meinem Test oft hatte) zu verhindern.
    // Die Bewegung wird nur ausgeführt, wenn der Gegner sein Ziel noch nicht erreicht hat.
    if (distance_To_Target > attack_Range)
    {
        is_Moving = true; // Der Gegner will sich bewegen
        // Normalisiert den Vektor: Macht den Pfeil zur reinen Richtung, indem seine Länge auf 1 gekürzt wird.
        // Dies ist der entscheidende Schritt für eine konstante Geschwindigkeit.
        float normalized_Direction_X = delta_Vector_X / distance_To_Target;
        float normalized_Direction_Y = delta_Vector_Y / distance_To_Target;

        // Holt die individuelle Geschwindigkeit dieses Gegners.
        float current_Movement_Speed = this->Get_Movement_Speed();

        // Berechnet die "Schrittgröße" für diesen einzelnen Frame.
        float movement_Step_Size = current_Movement_Speed * delta_Time;

        // Bewegt die Hitbox des Gegners um den kleinen Schritt in die korrekte Richtung.
        this->hitbox.x += normalized_Direction_X * movement_Step_Size;
        this->hitbox.y += normalized_Direction_Y * movement_Step_Size;

    }
    else
    {
        // Wenn du in Reichweite bist, bewege dich nicht.
        is_Moving = false;
    }
}

//Core Methoden
void Enemy_Base_Class::Tick(float delta_time)
{
    if (attack_Cooldown_Timer > 0)
    {
        attack_Cooldown_Timer -= delta_time;
    }
    // Wenn der Pathfinding die Position veränder dann this->is_Moving = true;
}
void Enemy_Base_Class::On_Collision(std::shared_ptr<Collidable> other)
{
    Collision_Type other_Type = other->Get_Collision_Type();

    switch(other_Type)
    {
        case Collision_Type::WALL:
        case Collision_Type::PLAYER:
        {
            if (this->is_Moving)
            {
                CollisionResponse::Resolve_Overlap(shared_from_this(), other);
            }

            if (other_Type == Collision_Type::PLAYER)
            {
                // 2. Prüfen, ob der Angriff bereit ist.
                if (attack_Cooldown_Timer <= 0)
                {
                    if (auto player = std::dynamic_pointer_cast<Player_Base_Class>(other))
                    {
                        // 3. Schaden austeilen und Cooldown zurücksetzen.
                        player->Take_Damage(this->enemy_Damage);
                        this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
                    }
                }
            }
            break;
        }
        case Collision_Type::PLAYER_PROJECTILE:
        {
            /*if(auto* projectile = dynamic_cast<Player_Projectile*>(other))
            {
                this->Take_Damage(projectile->Get_Damage());
            }
            CollisionResponse::Mark_For_Destruction(other);*/
            break;
        }
        case Collision_Type::ENEMY_SPAWNER:
        case Collision_Type::CONSUMABLE:
        {
            /* // Wir brauchen eine Basis-Klasse "Consumable", von der alle Items erben.
            if(auto* item = dynamic_cast<Consumables*>(other)) // Annahme: Es gibt eine Klasse Consumable
            {
                if(item->Is_Inventory_Item()) // Methode in Consumable, die den bool zurückgibt
                {
                    // Logik, um das Item dem Inventar hinzuzufügen
                    // inventory.Add(item);
                }
                else
                {
                    // Item wird sofort verwendet (z.B. Heilung)
                    item->Apply_Effect(this); // Jedes Item weiß selbst, was es tut
                }

                // In jedem Fall wird das Item aus der Welt entfernt
                CollisionResponse::Mark_For_Destruction(other);
            } */
            break;
        }

        default:
            break;
    }
}
void Enemy_Base_Class::Draw()
{
}
    void Enemy_Base_Class::Range_Attack()
{
}

    void Enemy_Base_Class::Melee_Attack()
{
    // Hier wird später die Logik für den Sweep-Angriff implementiert
    // (z.B. eine temporäre Hitbox vor dem Gegner erstellen).

    // Setze den Cooldown zurück, damit der Gegner nicht sofort wieder angreift.
    this->attack_Cooldown_Timer = this->attack_Cooldown_Duration;
}

}