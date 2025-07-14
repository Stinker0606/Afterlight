//
// Created by $Will on 22.06.2025.
//

#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H



#include "PlayerBaseClass.h"
#include "Animations.h"
#include "RepeatAnimation.h"
#include "Object_Manager.h"
#include "raylib.h" // Für Vector2

class Player_Class_One : public Player_Base_Class
{
private:
    // Deklariere die Größen-Member ohne Initialisierung hier.
    // Sie werden im Konstruktor initialisiert.
    Vector2 size_top_down;
    Vector2 size_left_right;
    Vector2 size_left_down_up;
    Vector2 size_right_down_up;

    RepeatAnimation* current_animation;

public:
    // Deklariere die RepeatAnimation Instanzen hier,
    // initialisiere sie aber NICHT direkt.
    // Idle
    RepeatAnimation Idle_Front;
    RepeatAnimation Idle_Back;
    RepeatAnimation Idle_Left;
    RepeatAnimation Idle_Right;
    RepeatAnimation Idle_Top_Right;
    RepeatAnimation Idle_Front_Right;
    RepeatAnimation Idle_Top_Left;
    RepeatAnimation Idle_Front_Left;
    // Moving
    RepeatAnimation Run_Front;
    RepeatAnimation Run_Back;
    RepeatAnimation Run_Left;
    RepeatAnimation Run_Right;
    RepeatAnimation Run_Top_Right;
    RepeatAnimation Run_Front_Right;
    RepeatAnimation Run_Top_Left;
    RepeatAnimation Run_Front_Left;


    Player_Class_One(Vector2 start_Position,Object_Manager& om);
    ~Player_Class_One();
    void Draw() override;

};


#endif //PLAYERCLASSONE_H
