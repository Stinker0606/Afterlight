//
// Created by Kruse on 23/05/2025.
//

#include "PlayerInput.h"
#include "../config.h.in"
#include <cmath> // für mmathematische Funktionen (wie sqrtf)

Player::Player(Vector2 start_Position, float move_Speed)
{
    position = start_Position;
    speed = move_Speed;

    //Steuerungstasten in die Config !
    key_Up = game::Config::key_Up;
    key_Down = game::Config::key_Down;
    key_Left = game::Config::key_Left;
    key_Right = game::Config::key_Right;
}

//Methode zur Steuerung der Bewegung. Wird jeden Frame aufgerufen
Player::Direction Player::Update()
{
    Vector2 movement_Direction = {0.0f, 0.0f};

    key_Melee_Attack = game::Config::key_Melee_Attack;

    bool up = IsKeyDown(key_Up);
    bool down = IsKeyDown(key_Down);
    bool left = IsKeyDown(key_Left);
    bool right = IsKeyDown(key_Right);

    if (up) movement_Direction.y -= 1.0f;
    if (down) movement_Direction.y += 1.0f;
    if (left) movement_Direction.x -= 1.0f;
    if (right) movement_Direction.x += 1.0f;

    // Bewegung normalisieren
    if (movement_Direction.x != 0.0f || movement_Direction.y != 0.0f)
    {
        float length = sqrtf(movement_Direction.x * movement_Direction.x + movement_Direction.y * movement_Direction.y);
        movement_Direction.x /= length;
        movement_Direction.y /= length;

        position.x += movement_Direction.x * speed * GetFrameTime();
        position.y += movement_Direction.y * speed * GetFrameTime();
    }

    if (IsKeyPressed(key_Melee_Attack) && !attacking)
    {
        attacking = true;
        attackTimer = attackDuration;
    }

    if (attacking)
    {
        attackTimer -= GetFrameTime();
        if (attackTimer <= 0.0f)
        {
            attacking = false;
            attackTimer = 0.0f;
        }
    }
    // Richtung zurückgeben
    if (movement_Direction.x < 0 && movement_Direction.y < 0) lastDirection = LeftUp;
    else if (movement_Direction.x > 0 && movement_Direction.y < 0) lastDirection = RightUp;
    else if (movement_Direction.x < 0 && movement_Direction.y > 0) lastDirection = LeftDown;
    else if (movement_Direction.x > 0 && movement_Direction.y > 0) lastDirection = RightDown;
    else if (movement_Direction.x < 0) lastDirection = Left;
    else if (movement_Direction.x > 0) lastDirection = Right;
    else if (movement_Direction.y < 0) lastDirection = Up;
    else if (movement_Direction.y > 0) lastDirection = Down;

    return lastDirection;
}
Rectangle Player::GetAttackHitbox() const
{
    if (!attacking) return { 0, 0, 0, 0 };

    float size = 20.0f;
    Vector2 hitboxPos = position;

    switch (lastDirection)
    {
        case Up:        hitboxPos.y -= size; break;
        case Down:      hitboxPos.y += size; break;
        case Left:      hitboxPos.x -= size; break;
        case Right:     hitboxPos.x += size; break;
        case LeftUp:    hitboxPos.x -= size; hitboxPos.y -= size; break;
        case RightUp:   hitboxPos.x += size; hitboxPos.y -= size; break;
        case LeftDown:  hitboxPos.x -= size; hitboxPos.y += size; break;
        case RightDown: hitboxPos.x += size; hitboxPos.y += size; break;
    }

    return Rectangle{ hitboxPos.x - size / 2, hitboxPos.y - size / 2, size, size };
}
void Player::Draw()
{
    Rectangle playerRect = {
        position.x - width / 2,
        position.y - height / 2,
        width,
        height
    };
    DrawRectangleRec(playerRect, BLUE);
}