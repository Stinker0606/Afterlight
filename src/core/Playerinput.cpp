//
// Created by Kruse on 23/05/2025.
//

#include "PlayerInput.h"
#include <cmath> // für mmathematische Funktionen (wie sqrtf)

Player::Player(Vector2 start_Position, float move_Speed)
{
    position = start_Position;
    speed = move_Speed;

    //Steuerungstasten in die Config !
    key_Up = KEY_W;
    key_Down = KEY_S;
    key_Left = KEY_A;
    key_Right = KEY_D;
}

//Methode zur Steuerung der Bewegung. Wird jeden Frame aufgerufen
void Player::Update()
{
    //Bewegungsrichtung ohne Werte
    Vector2 movement_Direction;
    movement_Direction.x = 0.0f;
    movement_Direction.y = 0.0f;

    // Abfragen welche Tasten gedrückt werden
    if (IsKeyDown(key_Up))
    {
        movement_Direction.y -= 1.0f; //nach oben
    }
    if (IsKeyDown(key_Down))
    {
        movement_Direction.y += 1.0f; //nach unten
    }
    if (IsKeyDown(key_Left))
    {
        movement_Direction.x -= 1.0f; //nach links
    }
    if (IsKeyDown(key_Right))
    {
        movement_Direction.x += 1.0f; // nach rechts
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
