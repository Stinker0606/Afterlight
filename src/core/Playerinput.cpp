//
// Created by Kruse on 23/05/2025.
//

#include "PlayerInput.h"
#include "../config.h.in"
#include <cmath> // für mathematische Funktionen (wie sqrtf)

namespace game::core
{
    // Spieler Input
    Player::Player(Vector2 start_Position, float move_Speed)
            : position(start_Position), speed(move_Speed)
    {
        //Steuerungstasten aus globaler Config

        key_Up = game::Config::key_Up;
        key_Down = game::Config::key_Down;
        key_Left = game::Config::key_Left;
        key_Right = game::Config::key_Right;
    }

    // Prüft Tasteneingabe, normalisiert bewegung, setzt Spieler Richtung
    Player::Direction Player::Update(DT::timemachine& deltaTimeMachine)
    {
        Vector2 movement = {0.0f, 0.0f};

        // Tasteneingabe prüfen
        if (IsKeyDown(key_Up))    movement.y -= 1.0f;
        if (IsKeyDown(key_Down))  movement.y += 1.0f;
        if (IsKeyDown(key_Left))  movement.x -= 1.0f;
        if (IsKeyDown(key_Right)) movement.x += 1.0f;

        // Bewegung normalisieren (diagonal = gleiche Geschwindigkeit)
        if (movement.x != 0.0f || movement.y != 0.0f)
        {
            float length = sqrtf(movement.x * movement.x + movement.y * movement.y);
            movement.x /= length;
            movement.y /= length;

            float dt = deltaTimeMachine.Get_Dt();
            position.x += movement.x * speed * dt;
            position.y += movement.y * speed * dt;
        }

        // Richtung setzen
        if (movement.x < 0 && movement.y < 0) lastDirection = LeftUp;
        else if (movement.x > 0 && movement.y < 0) lastDirection = RightUp;
        else if (movement.x < 0 && movement.y > 0) lastDirection = LeftDown;
        else if (movement.x > 0 && movement.y > 0) lastDirection = RightDown;
        else if (movement.x < 0) lastDirection = Left;
        else if (movement.x > 0) lastDirection = Right;
        else if (movement.y < 0) lastDirection = Up;
        else if (movement.y > 0) lastDirection = Down;

        return lastDirection;
    }
}

