//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_PLAYERINPUT_H
#define RAYLIBSTARTER_PLAYERINPUT_H

#include "raylib.h"
#include "DeltaTimeMachine.h"

//Vieles hier ist Teamspezifisch
namespace game::core
{
    class Player
    {
    public:
        enum Direction
        {
            Up, Down, Left, Right, LeftDown, RightDown, LeftUp, RightUp
        };

        // Konstruktor initialisiert Spieler mit Start Position und Bewegungsgeschwindigkeit
        Player(Vector2 start_Position, float move_Speed);

        // Wird mit jedem Frame aufgerufen, um Position zu aktualisieren
        Direction Update(DT::timemachine& deltaTimeMachine);

        // Zeichnet den Spieler
        void Draw();

        // Gibt die aktuelle Spielerposition zurück
        Vector2 GetPosition() const;

    private:

        // Aktuelle Position des Spielers
        Vector2 position;

        // Bewegungsgeschwindigkeit
        float speed;

        // Tastenzuweisung für die Bewegung
        int key_Up;
        int key_Down;
        int key_Left;
        int key_Right;

        // Taste für den Nahkampfangriff
        int key_Melee_Attack;

        // Angriffssteuerung
        bool attacking=false;
        float attackDuration=0.3f;
        float attackTimer = 0.0f;

        // Zuletzt registrierte Bewegungsrichtung
        Direction lastDirection = Down;
    };
}

#endif //RAYLIBSTARTER_PLAYERINPUT_H