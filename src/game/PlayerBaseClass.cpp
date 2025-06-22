//
// Created by Kruse on 23/05/2025.
//

#include "PlayerBaseClass.h"

Player_Base_Class::Player_Base_Class(int max_Health, float movement_Speed, int damage, const char* sprite_Path,
    int frame_Width, int frame_Height, int anim_Steps, int anim_Speed, Vector2 start_Position, Collision_Manager* manager)

    : Actor(std::make_shared<game::core::SpriteAnimated>(std::make_shared<game::core::Texture2D>(sprite_Path),
            (float)frame_Width, (float)frame_Height, 1,anim_Steps, anim_Speed)),

    Collidable()
{
    // Werte aus den Parametern in die Member-Variablen speichern
    player_Max_Health = max_Health;
    player_Health = (float)player_Max_Health;
    player_Movement_Speed = movement_Speed;
    player_Damage = damage;

    // Initialisiere die Hitbox und die Sprite-Position
    player_Hitbox = { start_Position.x, start_Position.y, (float)frame_Width, (float)frame_Height };
    sprite()->pos_x = (int)start_Position.x;
    sprite()->pos_y = (int)start_Position.y;

    manager_Ptr = manager;
    if (manager_Ptr)
    {
        manager_Ptr->Regist_Object(this);
    }

    melee_Cooldown = 0.0f;
    ranged_Cooldown = 0.0f;
    inventory_Is_Full = false;
    facing_Direction = Facing_Direction::DOWN;
    is_Moving = false;
}

Player_Base_Class::~Player_Base_Class()
{
    if (manager_Ptr)
    {
        manager_Ptr->Unregist_Object(this);
    }
}

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

    if (IsKeyPressed(game::Config::key_Use_Item) && inventory_Is_Full)
    {
        Use_Item();
    }
}

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

    // Animations-Steuerung
    auto anim_Sprite = std::dynamic_pointer_cast<game::core::SpriteAnimated>(this->sprite());
    if (!anim_Sprite) return;

    // Synchronisiere die Sprite-Position mit der Hitbox-Position
    anim_Sprite->pos_x = player_Hitbox.x;
    anim_Sprite->pos_y = player_Hitbox.y;

    if (is_Moving)
    {
        // Wähle den richtigen Animations-State basierend auf der Blickrichtung
        switch (facing_Direction)
        {
            case Facing_Direction::DOWN: anim_Sprite->nextState(0); break;
            case Facing_Direction::UP: anim_Sprite->nextState(1); break;
            case Facing_Direction::LEFT: anim_Sprite->nextState(2); break;
            case Facing_Direction::RIGHT: anim_Sprite->nextState(3); break;
            // Diagonale Fälle könnten auf Links/Rechts abgebildet werden
            case Facing_Direction::DOWN_LEFT: case Facing_Direction::UP_LEFT: anim_Sprite->nextState(2); break;
            case Facing_Direction::DOWN_RIGHT: case Facing_Direction::UP_RIGHT: anim_Sprite->nextState(3); break;
        }
    }
    else
    {
        // Wenn wir stehen, könnten wir einen Idle-State setzen, z.B. State 0
         anim_Sprite->nextState(0);
    }

    // Rufe die Update-Methode des Sprites auf, damit es seine Frames weiterschaltet
    anim_Sprite->Update();
}

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

void Player_Base_Class::Melee_Attack()
{
	melee_Cooldown = 0.0f;
}
void Player_Base_Class::Ranged_Attack()
{
	ranged_Cooldown = 0.0f;
}
void Player_Base_Class::Update_Previous_Position()
{
    previous_Position.x = player_Hitbox.x;
    previous_Position.y = player_Hitbox.y;
}

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

void Player_Base_Class::Stop_Movement()
{
    player_Hitbox.x = previous_Position.x;
    player_Hitbox.y = previous_Position.y;
}

Rectangle Player_Base_Class::Get_Hitbox() const
{
    return player_Hitbox;
}

Collision_Type Player_Base_Class::Get_Collision_Type() const
{
    return Collision_Type::PLAYER;
}