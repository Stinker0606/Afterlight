//
// Created by $Will on 22.06.2025.
//

/*

#include "PlayerClassOne.h"
#include "../config.h.in"
#include "raylib.h"

Player_Class_One::Player_Class_One(Vector2 start_Position,Object_Manager& om)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
    game::Config::player_Class_One_Damage, start_Position,om),

    // Idle
        Idle_Front{size, "assets/graphics/animations/player/idle/MC_Idle_Front.png", 80, 80},
        Idle_Back{size, "assets/graphics/animations/player/idle/MC_Idle_Back.png", 80, 80},
        Idle_Left{size, "assets/graphics/animations/player/idle/MC_Idle_Left.png", 80, 80},
        Idle_Right{size, "assets/graphics/animations/player/idle/MC_Idle_Right.png", 80, 80},
        //Idle_Top_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Idle_Front_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Idle_Top_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Idle_Front_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER}

    // Moving
        Run_Front{size, "assets/graphics/animations/player/walk/MC_Walkcycle_Front-Sheet.png", 40, 40},
        Run_Back{size, "assets/graphics/animations/player/walk/MC_Walkcycle_Back-Sheet.png", 40, 40},
        Run_Left{size, "assets/graphics/animations/player/walk/MC_Walkcycle_Left-Sheet.png", 40, 40},
        Run_Right{size, "assets/graphics/animations/player/walk/MC_Walkcycle_Right-Sheet.png", 40, 40}
        //Run_Top_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Run_Front_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Run_Top_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Run_Front_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER}

    //Attack Throw
        //Throw_Front{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Back{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Top_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Front_Right{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Top_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER},
        //Throw_Front_Left{size, "PLACEHOLDER", PLACEHOLDER, PLACEHOLDER}
{



    // Set the initial animation
    current_animation = &Idle_Front;
}

Player_Class_One::~Player_Class_One() {
    // If RepeatAnimation handles LoadTexture/UnloadTexture internally,
    // its destructors will be called automatically when Player_Class_One is destroyed.
    // No explicit UnloadTexture calls needed here unless RepeatAnimation *doesn't*
    // handle resource cleanup in its destructor.
}

void Player_Class_One::Draw() {
    // 1. Wähle die Animation basierend auf dem Zustand aus der Basisklasse.
    // Die Basisklasse `PlayerBaseClass` aktualisiert `is_Moving` und `facing_Direction` in ihrer `Tick`-Methode.
    if (this->is_Moving) {
        // Wähle die LAUF-Animation für die aktuelle Richtung
        switch (this->facing_Direction) {
            case Facing_Direction::UP:         current_animation = &Run_Back; break;
            case Facing_Direction::DOWN:       current_animation = &Run_Front; break;
            case Facing_Direction::LEFT:       current_animation = &Run_Left; break;
            case Facing_Direction::RIGHT:      current_animation = &Run_Right; break;
            //case Facing_Direction::UP_LEFT:    current_animation = &Run_Top_Left; break;
            //case Facing_Direction::UP_RIGHT:   current_animation = &Run_Top_Right; break;
            //case Facing_Direction::DOWN_LEFT:  current_animation = &Run_Front_Left; break;
            //case Facing_Direction::DOWN_RIGHT: current_animation = &Run_Front_Right; break;
        }
    } else {
        // Wähle die IDLE-Animation für die letzte bekannte Richtung
        switch (this->facing_Direction) {
            case Facing_Direction::UP:         current_animation = &Idle_Back; break;
            case Facing_Direction::DOWN:       current_animation = &Idle_Front; break;
            case Facing_Direction::LEFT:       current_animation = &Idle_Left; break;
            case Facing_Direction::RIGHT:      current_animation = &Idle_Right; break;
            //case Facing_Direction::UP_LEFT:    current_animation = &Idle_Top_Left; break;
            //case Facing_Direction::UP_RIGHT:   current_animation = &Idle_Top_Right; break;
            //case Facing_Direction::DOWN_LEFT:  current_animation = &Idle_Front_Left; break;
            //case Facing_Direction::DOWN_RIGHT: current_animation = &Idle_Front_Right; break;
        }
    }

    // 2. Zeichne den aktuellen Frame.
    current_animation->Draw_Current_Frame(this->Get_Player_Center());

    // 3. Spiele IMMER die Animation ab (egal ob Idle oder Run).
    current_animation->Next_Frame();
}
*/