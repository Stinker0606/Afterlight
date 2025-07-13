//
// Created by Kruse on 23/05/2025.
//

#include <valarray>
#include "EnemyBaseClass.h"
#include "CollisionManager.h"
#include "CollisionResponse.h"
#include "PlayerBaseClass.h"

namespace enemy
{
Enemy_Base_Class::Enemy_Base_Class(std::string name, int health, float movement_speed, int damage, int value,
    const char* sprite_path, const char* projectile_sprite_path,Vector2 start_position, int width, int height, float cooldown_Duration)
    : enemy_Name(name), enemy_Health(health), enemy_Movement_Speed(movement_speed), enemy_Damage(damage),
      enemy_Value(value),attack_Cooldown_Duration(cooldown_Duration), attack_Cooldown_Timer(0.0f), is_Moving(false)
    {
    hitbox = {start_position.x, start_position.y, (float)width, (float)height};
    sprite = LoadTexture(sprite_path);
    }

Enemy_Base_Class::~Enemy_Base_Class()
{
    UnloadTexture(sprite);
}

void Enemy_Base_Class::Take_Damage(int damage_amount)
{
    enemy_Health -= damage_amount;
}

void Enemy_Base_Class::Pathfinding(float target_Position_X, float target_Position_Y, float delta_Time)
{
    // Berechnet den Richtungs-Vektor vom Gegner zum Ziel.
    float delta_Vector_X = target_Position_X - this->hitbox.x;
    float delta_Vector_Y = target_Position_Y - this->hitbox.y;

    // Berechnet die exakte Distanz zum Ziel.
    float distance_To_Target = std::sqrt(delta_Vector_X * delta_Vector_X + delta_Vector_Y * delta_Vector_Y);

    // Sicherheitscheck, um eine Division durch Null (Fehler den ich bei meinem Test oft hatte) zu verhindern.
    // Die Bewegung wird nur ausgeführt, wenn der Gegner sein Ziel noch nicht erreicht hat.
    if (distance_To_Target > 0.01f)
    {
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
    is_Moving= true;
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
void Enemy_Base_Class::On_Collision(Collidable* other)
{
    Collision_Type other_Type = other->Get_Collision_Type();

    switch(other_Type)
    {
        case Collision_Type::WALL:
        case Collision_Type::ENEMY_SPAWNER:
        case Collision_Type::PLAYER:
        {
            if (this->is_Moving)
            {
                CollisionResponse::Resolve_Overlap(this, other);
            }

            if (other_Type == Collision_Type::PLAYER)
            {
                // 2. Prüfen, ob der Angriff bereit ist.
                if (attack_Cooldown_Timer <= 0)
                {
                    if (auto* player = dynamic_cast<Player_Base_Class*>(other))
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
}