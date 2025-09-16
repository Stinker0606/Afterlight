//
// Created by Kruse on 23/05/2025.
//

#include <iostream>
#include "PlayerBaseClass.h"
#include "CollisionResponse.h"
#include "../game/interactables/interact_list.h"
#include "../game/interactables/MeleeHitbox.h"
#include "Store.h"

// Konstruktor
Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, Vector2 start_Position, Object_Manager* om)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      previous_Position(start_Position), melee_Cooldown(0.0f), ranged_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false), p_om_(om)
{
    hitbox={start_Position.x,start_Position.y,game::Config::Player_Hitbox_Width,game::Config::Player_Hitbox_Height};
    // 2. Registriere Objekt beim Manager

}

// Destruktor
Player_Base_Class::~Player_Base_Class()
{

}

void Player_Base_Class::Set_Object_Manager(Object_Manager* new_om)
{
    this->p_om_ = new_om;
}

// Phase 1 :: Player input Prüfung
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
}

// Kümmert sich um alles außer Bewegung
void Player_Base_Class::UpdateTimersAndEffects(float delta_time)
{
    // HP-Drain Logik
    if (game::Config::enable_Health_Drain && player_Health > 0)
    {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
        if (player_Health < 0) {
            player_Health = 0;
        }
    }

    // Cooldown-Logik
    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (ranged_Cooldown > 0) ranged_Cooldown -= delta_time;
}

// Phase 2 :: Verwaltung für alles was das Objekt über eine gewisse Zeit machen soll
void Player_Base_Class::Tick(float delta_time)
{
    Update_Previous_Position();

    Vector2 move_Direction = {0.0f, 0.0f};
    if (IsKeyDown(game::Config::key_Up))    move_Direction.y -= 1.0f;
    if (IsKeyDown(game::Config::key_Down))  move_Direction.y += 1.0f;
    if (IsKeyDown(game::Config::key_Left))  move_Direction.x -= 1.0f;
    if (IsKeyDown(game::Config::key_Right)) move_Direction.x += 1.0f;

    is_Moving = (move_Direction.x != 0.0f || move_Direction.y != 0.0f);
    if (is_Moving)
    {
        move_Direction = Vector2Normalize(move_Direction);
    }

    hitbox.x += (move_Direction.x * player_Movement_Speed * delta_time);
    hitbox.y += (move_Direction.y * player_Movement_Speed * delta_time);

    // Weltgrenzen
    if (hitbox.x < game::Config::kWorldBoundsMinX) hitbox.x = game::Config::kWorldBoundsMinX;
    if (hitbox.y < game::Config::kWorldBoundsMinY) hitbox.y = game::Config::kWorldBoundsMinY;
    if (hitbox.x + hitbox.width > game::Config::kWorldBoundsMaxX) hitbox.x = game::Config::kWorldBoundsMaxX - hitbox.width;
    if (hitbox.y + hitbox.height > game::Config::kWorldBoundsMaxY) hitbox.y = game::Config::kWorldBoundsMaxY - hitbox.height;

    player_Pos.x=hitbox.x;
    player_Pos.y=hitbox.y;

    Update_Facing_Direction();
}

// Phase 3 :: Kollisionsreaktion falls der Collisionmanager eine Kollision mit einem anderen Objekt feststellt
void Player_Base_Class::On_Collision(std::shared_ptr<Collidable> other)
{
    // Prüfe, ob es sich um einen Push_Block handelt. Deine Logik hat hier Vorrang.
    if (auto push_block = std::dynamic_pointer_cast<Push_Block>(other))
    {
        // Deine funktionierende Push-Logik
        Vector2 move_direction = { hitbox.x - previous_Position.x, hitbox.y - previous_Position.y };
        if (fabs(move_direction.x) > fabs(move_direction.y)) {
            move_direction.y = 0;
            move_direction.x = (move_direction.x > 0) ? 1 : -1;
        } else {
            move_direction.x = 0;
            move_direction.y = (move_direction.y > 0) ? 1 : -1;
        }
        push_block->Push(move_direction);

        // Setze den Spieler auf seine alte Position zurück, um das "Kleben" am Block zu verhindern
        hitbox.x = previous_Position.x;
        hitbox.y = previous_Position.y;
    }
    else
    {
        // Für ALLE ANDEREN soliden Objekte (Wände, Gegner, Spawner), benutze die neue "Gleiten"-Logik.
        Collision_Type otherType = other->Get_Collision_Type();
        if (otherType == Collision_Type::WALL ||
            otherType == Collision_Type::ENEMY_SPAWNER ||
            otherType == Collision_Type::ENEMY)
        {
            CollisionResponse::Resolve_Overlap(shared_from_this(), other);
        }
    }
}

// Draw Methode ist noch nicht klar, wie das mit der Visualisierung laufen wird
void Player_Base_Class::Draw()
{
    DrawTexture(this->maintex, this->hitbox.x,hitbox.y,WHITE);
}

void Player_Base_Class::Melee_Attack()
{

}

void Player_Base_Class::Ranged_Attack()
{

}

// Funktion für die Tick Methode welche die aktuelle Position speichert, falls das Objekt zurück gesetzt werden soll
void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = hitbox.x;
    previous_Position.y = hitbox.y;
}

// Methode aus der Tick welche die aktuelle Blickrichtung zurück geben soll. Wird später fürs Zeichnen und für die
// Angriffe genutzt
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

// Getter für Player Hittbox und Collision Type
Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}

Vector2 Player_Base_Class::Get_Player_Pos() {
    return this->player_Pos;
}
void Player_Base_Class::Take_Damage(int damage_amount)
{
    player_Health -= damage_amount;
}
Vector2 Player_Base_Class::Get_Player_Center() {
    Vector2 player_Center;
    player_Center.x = player_Pos.x+hitbox.width/2;
    player_Center.y = player_Pos.y+hitbox.height/2;
    return player_Center;
}
