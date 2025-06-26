//
// Created by Kruse on 23/05/2025.
//

#include "PlayerBaseClass.h"

// Konstruktor
Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, int damage, Vector2 start_Position,
     Collision_Manager* manager)
    : player_Max_Health(max_Health), player_Health((float)max_Health), player_Movement_Speed(movement_Speed),
      player_Damage(damage), player_Hitbox({ start_Position.x, start_Position.y, 16.0f, 16.0f }),
      previous_Position(start_Position), manager_Ptr(manager), melee_Cooldown(0.0f), ranged_Cooldown(0.0f),
      inventory_Is_Full(false), facing_Direction(Facing_Direction::DOWN), is_Moving(false)
{
    // 2. Registriere Objekt beim Manager
    if (manager_Ptr)
    {
        manager_Ptr->Regist_Object(this);
    }
}

// Destruktor
Player_Base_Class::~Player_Base_Class()
{
    if (manager_Ptr)
    {
        manager_Ptr->Unregist_Object(this);
    }
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

    /*if (IsKeyPressed(game::Config::key_Use_Item) && inventory_Is_Full)
    {
        Use_Item();
    }*/
}

// Phase 2 :: Verwaltung für alles was das Objekt über eine gewisse Zeit machen soll
void Player_Base_Class::Tick(float delta_time)
{
    if (game::Config::enable_Health_Drain)
    {
        player_Health -= game::Config::player_Health_Drain_Rate * delta_time;
    }
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

    player_Hitbox.x += move_Direction.x * player_Movement_Speed * delta_time;
    player_Hitbox.y += move_Direction.y * player_Movement_Speed * delta_time;

    Update_Facing_Direction();

    if (melee_Cooldown > 0) melee_Cooldown -= delta_time;
    if (ranged_Cooldown > 0) ranged_Cooldown -= delta_time;
}

// Phase 3 :: Kollisionsreaktion falls der Collisionmanager eine Kollision mit einem anderen Objekt feststellt
void Player_Base_Class::On_Collision(Collidable* other)
{
	Collision_Type otherType = other->Get_Collision_Type();

    if (otherType == Collision_Type::WALL ||
        otherType == Collision_Type::ENEMY_SPAWNER ||
        otherType == Collision_Type::ENEMY)
    {
		Rectangle wall_Hitbox = other->Get_Hitbox();
        if (CheckCollisionRecs({player_Hitbox.x, previous_Position.y, player_Hitbox.width, player_Hitbox.height}, wall_Hitbox))
        {
            player_Hitbox.y = previous_Position.y;
        }
        if (CheckCollisionRecs({previous_Position.x, player_Hitbox.y, player_Hitbox.width, player_Hitbox.height}, wall_Hitbox))
        {
            player_Hitbox.x = previous_Position.x;
		}
	}
}

// Draw Methode ist noch nicht klar, wie das mit der Visualisierung laufen wird
void Player_Base_Class::Draw()
{
    DrawTexture(this->maintex, this->player_Hitbox.x,player_Hitbox.y,WHITE);
}

// Um die beiden Attack Methoden weiter auszuarbeiten, braucht es die passenden Klassen
void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown = 0.0f;
}
void Player_Base_Class::Ranged_Attack()
{
	ranged_Cooldown = 0.0f;
}

// Funktion für die Tick Methode welche die aktuelle Position speichert, falls das Objekt zurück gesetzt werden soll
void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = player_Hitbox.x;
    previous_Position.y = player_Hitbox.y;
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
Rectangle Player_Base_Class::Get_Hitbox() const
{
    return player_Hitbox;
}

Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}