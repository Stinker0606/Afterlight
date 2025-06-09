//
// Created by Kruse on 23/05/2025.
//
#include "PlayerClass1.h"
#include "../config.h.in"

Player_Class_1::Player_Class_1(Vector2 start_position)
    // Ruft den Konstruktor der Basisklasse auf und füllt ihn mit Werten aus der Config
    : Player_Base_Class(game::Config::player_Class_1_Max_Health, game::Config::player_Class_1_Movement_Speed,
          game::Config::player_Class_1_Damage, start_position)
{
    this->sprite = LoadTexture(game::Config::player_Class_1_Sprite_Path);
}

Player_Class_1::~Player_Class_1()
{
    UnloadTexture(this->sprite);
}

void Player_Class_1::Attack()
{
    // Hier kommt die Logik für den Angriff rein
}

void Player_Class_1::Draw()
{
    // Zeichne das Sprite des Kriegers an der Position seiner Hitbox
    DrawTexture(this->sprite, this->player_Hitbox.x, this->player_Hitbox.y, WHITE);
}