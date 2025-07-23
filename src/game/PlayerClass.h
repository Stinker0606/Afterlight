#ifndef PLAYERCLASSONE_H
#define PLAYERCLASSONE_H



#include "PlayerBaseClass.h"
#include "Animations.h"
#include "RepeatAnimation.h"

class Player_Class : public Player_Base_Class
{
private:
    Vector2 size{100,100};

    RepeatAnimation* current_animation;

public:
    // Deklariere die RepeatAnimation Instanzen hier,
    // initialisiere sie aber NICHT direkt.

    // Idle
    RepeatAnimation Idle_Front;
    RepeatAnimation Idle_Back;
    RepeatAnimation Idle_Left;
    RepeatAnimation Idle_Right;
    //RepeatAnimation Idle_Top_Right;
    //RepeatAnimation Idle_Front_Right;
    //RepeatAnimation Idle_Top_Left;
    //RepeatAnimation Idle_Front_Left;

    // Moving
    RepeatAnimation Run_Front;
    RepeatAnimation Run_Back;
    RepeatAnimation Run_Left;
    RepeatAnimation Run_Right;
    //RepeatAnimation Run_Top_Right;
    //RepeatAnimation Run_Front_Right;
    //RepeatAnimation Run_Top_Left;
    //RepeatAnimation Run_Front_Left;

    // Attack-Throw
    //RepeatAnimation Throw_Front;
    //RepeatAnimation Throw_Back;
    //RepeatAnimation Throw_Left;
    //RepeatAnimation Throw_Right;
    //RepeatAnimation Throw_Top_Right;
    //RepeatAnimation Throw_Front_Right;
    //RepeatAnimation Throw_Top_Left;
    //RepeatAnimation Throw_Front_Left;

    Player_Class(Vector2 start_Position,Object_Manager& om);
    ~Player_Class();
    void Draw() override;

};


#endif //PLAYERCLASSONE_H
