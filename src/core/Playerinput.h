//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYERINPUT_H
#define RAYLIBSTARTER_PLAYERINPUT_H


#include "raylib.h"


class Player
{
protected:
    float width = 20.0f;
    float height = 30.0f;
public:

    enum Direction
    {
        Up, Down, Left, Right, LeftDown, RightDown, LeftUp, RightUp
    };

    // Konstroktor initialisiert Spieler mit Start Position und Bewegungsgeschwindigkeit
    Player(Vector2 start_Position, float move_Speed);

    // Wird mit jedem Frame aufgerufen, um Position zu aktualisieren
    Direction Update();

    void DrawRectangle(Vector2 vector2, int pos_y, Color color);

    //Zeichnet den Spieler
    void Draw();
    Vector2 GetPosition() const { return position; }

    bool IsAttacking() const { return attacking; }
    Rectangle GetAttackHitbox() const;

private:

    //Aktuelle Position des Spielers
    Vector2 position;

    //Bewegungsgeschwindigkeit
    float speed;

    //Tastenzuweißung für die Bewegung
    int key_Up;
    int key_Down;
    int key_Left;
    int key_Right;

    //Taste für den Angriff
    int key_J;

    bool attacking=false;
    float attackDuration=0.3f;
    float attackTimer = 0.0f;

    Direction lastDirection = Down;
};


#endif //RAYLIBSTARTER_PLAYERINPUT_H
