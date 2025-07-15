//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H

#include "PlayerBaseClass.h"
#include "Animations.h"
#include "RepeatAnimation.h"
#include "raylib.h" // Für Vector2

class Player_Class_One : public Player_Base_Class
{
private:
    // Deklariere die Größen-Member ohne Initialisierung hier.
    // Sie werden im Konstruktor initialisiert.
    Vector2 size_top_down;
    Vector2 size_left_right;


    RepeatAnimation* current_animation;

public:
    // Deklariere die RepeatAnimation Instanzen hier,
    // initialisiere sie aber NICHT direkt.
    // Idle
    RepeatAnimation Idle_Front;
    RepeatAnimation Idle_Back;
    RepeatAnimation Idle_Left;
    RepeatAnimation Idle_Right;

    // Moving
    RepeatAnimation Run_Front;
    RepeatAnimation Run_Back;
    RepeatAnimation Run_Left;
    RepeatAnimation Run_Right;



    Player_Class_One(Vector2 start_Position);
    ~Player_Class_One();
    void Draw() override;

};

#endif //PLAYERCLASSONE_H