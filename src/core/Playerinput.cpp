//
// Created by Kruse on 23/05/2025.
//

#include "PlayerInput.h"
#include "../config.h.in"
#include <cmath> // für mathematische Funktionen (wie sqrtf)

//Vieles hier ist Teamspezifisch
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

        key_Melee_Attack = game::Config::key_Melee_Attack;
    }

    // Prüft Tasteneingabe, normalisiert bewegung, kontrolliert Angriffsdauer, setzt Spieler Richtung
    Player::Direction Player::Update(DT::timemachine& deltaTimeMachine)
    {
        Vector2 movement = {0.0f, 0.0f};

        key_Melee_Attack = game::Config::key_Melee_Attack;

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

        // Nahkampfangriff starten
        if (IsKeyPressed(key_Melee_Attack) && !attacking)
        {
            attacking = true;
            attackTimer = attackDuration;
        }

        // Angriff aktualisieren
        if (attacking)
        {
            attackTimer -= deltaTimeMachine.Get_Dt();
            if (attackTimer <= 0.0f)
            {
                attacking = false;
                attackTimer = 0.0f;
            }
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

    // Bestimmt die Nahkampfhittbox je nach Richtung
    Rectangle Player::GetAttackHitbox() const
    {
        if (!attacking) return { 0, 0, 0, 0 };

        const float size = 20.0f;
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

    // Bestimmt Erscheinungsbild des Spielers
    void Player::Draw()
    {
        Rectangle playerRect =
                {
                        position.x - width / 2,
                        position.y - height / 2,
                        width,
                        height
                };
        DrawRectangleRec(playerRect, BLUE);
    }
}

    // Wenn der Spieler sich bewegt
    if (movement_Direction.x != 0.0f || movement_Direction.y != 0.0f)
    {
        // Die Bewegung wird normalisiert, damit diagonale Bewegung nicht schneller ist
        // Richting wird beibehalten, Stärke (Länge) des Vectors auf 1 gesetzt
        float vectorLength = sqrtf((movement_Direction.x * movement_Direction.x) +
                                   (movement_Direction.y * movement_Direction.y));

        movement_Direction.x = movement_Direction.x / vectorLength;
        movement_Direction.y = movement_Direction.y / vectorLength;
    }

    // Die Bewegung wird mit der Zeit pro Frame verrechnet,
    // damit Geschwindigkeit unabhängig von der Framerate bleibt
    float deltaTime = GetFrameTime();

    //Neue Position berechnen
    position.x += movement_Direction.x * speed * deltaTime;
    position.y += movement_Direction.y * speed * deltaTime;

}
