//
// Created by $Will on 22.06.2025.
//

#include "PlayerClassOne.h"
#include "../config.h.in"
#include "raylib.h" // For IsKeyDown

Player_Class_One::Player_Class_One(Vector2 start_Position)
    : Player_Base_Class(game::Config::player_Class_One_Max_Health, game::Config::player_Class_One_Movement_Speed,
        game::Config::player_Class_One_Damage, start_Position),
      // --- START OF MEMBER INITIALIZER LIST ---
      // 1. Initialize the size Vector2 members first
      size_top_down{32,48},
      size_left_right{33, 48},
      size_left_down_up{32,48},
      size_right_down_up{32,48},
      // 2. Then, initialize the RepeatAnimation members, using the now-initialized size members
      // Idle
      Idle_Front{size_top_down, "assets/graphics/animations/player/idle/Maincharacter_Idle_Front_Spritesheet.png", 10, 10},
      Idle_Back{size_top_down, "assets/graphics/animations/player/idle/Maincharacter_Idle_back_Spritesheet.png", 10, 10},
      Idle_Left{size_left_right, "assets/graphics/animations/player/idle/Maincharacter_Idle_Left_Spritesheet.png", 10, 10},
      Idle_Right{size_left_right, "assets/graphics/animations/player/idle/Maincharacter_Idle_Right_Spritesheet.png", 10, 10},
      Idle_Top_Left{size_left_down_up, "assets/graphics/animations/player/idle/Maincharacter_Idle_Side3_Spritesheet.png", 10, 10},
      Idle_Top_Right{size_right_down_up, "assets/graphics/animations/player/idle/Maincharacter_Idle_Side2_Spritesheet.png", 10, 10},
      Idle_Front_Left{size_left_down_up, "assets/graphics/animations/player/idle/Maincharacter_Idle_Side4_Spritesheet.png", 10, 10},
      Idle_Front_Right{size_right_down_up, "assets/graphics/animations/player/idle/Maincharacter_Idle_Side1_Spritesheet.png", 10, 10},
      // Moving
      Run_Front{size_top_down, "assets/graphics/animations/player/walk/MC_Walkcycle_Front.png", 10, 10},
      Run_Back{size_top_down, "assets/graphics/animations/player/walk/MC_Walkcycle_Back.png", 10, 10},
      Run_Left{size_left_right, "assets/graphics/animations/player/walk/MC_Walkcycle_Left.png", 10, 10},
      Run_Right{size_left_right, "assets/graphics/animations/player/walk/MC_Walkcycle_Right.png", 10, 10},
      Run_Top_Left{size_left_down_up, "assets/graphics/animations/player/walk/MC_Walkcycle_BackLeft.png", 10, 10},
      Run_Top_Right{size_right_down_up, "assets/graphics/animations/player/walk/MC_Walkcycle_BackRight.png", 10, 10},
      Run_Front_Left{size_left_down_up, "assets/graphics/animations/player/walk/MC_Walkcycle_FrontLeft.png", 10, 10},
      Run_Front_Right{size_right_down_up, "assets/graphics/animations/player/walk/MC_Walkcycle_FrontRight.png", 10, 10}
      // --- END OF MEMBER INITIALIZER LIST ---
{
    // Hitbox Player
    this->hitbox.width = 31.0f;
    this->hitbox.height = 46.0f;

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
            case Facing_Direction::UP_LEFT:    current_animation = &Run_Top_Left; break;
            case Facing_Direction::UP_RIGHT:   current_animation = &Run_Top_Right; break;
            case Facing_Direction::DOWN_LEFT:  current_animation = &Run_Front_Left; break;
            case Facing_Direction::DOWN_RIGHT: current_animation = &Run_Front_Right; break;
        }
    } else {
        // Wähle die IDLE-Animation für die letzte bekannte Richtung
        switch (this->facing_Direction) {
            case Facing_Direction::UP:         current_animation = &Idle_Back; break;
            case Facing_Direction::DOWN:       current_animation = &Idle_Front; break;
            case Facing_Direction::LEFT:       current_animation = &Idle_Left; break;
            case Facing_Direction::RIGHT:      current_animation = &Idle_Right; break;
            case Facing_Direction::UP_LEFT:    current_animation = &Idle_Top_Left; break;
            case Facing_Direction::UP_RIGHT:   current_animation = &Idle_Top_Right; break;
            case Facing_Direction::DOWN_LEFT:  current_animation = &Idle_Front_Left; break;
            case Facing_Direction::DOWN_RIGHT: current_animation = &Idle_Front_Right; break;
        }
    }

    // 2. Zeichne den aktuellen Frame.
    current_animation->Draw_Current_Frame(this->player_Pos);

    // 3. Spiele IMMER die Animation ab (egal ob Idle oder Run).
    current_animation->Next_Frame();
}