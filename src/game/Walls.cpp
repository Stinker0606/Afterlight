//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
Walls::Walls(Vector2 position, Vector2 size)
{

    this->size=size;
    this->pos=position;
    Rectangle hb(this->pos.x, this->pos.y, this->size.x, this->size.y);
    this->hitbox=hb;

}

void Walls::Draw() {}

CollisionType Walls::Get_Collision_Type() const
{
    return CollisionType::WALL;
}

void Walls::On_Collision(Collidable* other)
{
    // Eine Wand reagiert normalerweise nicht auf eine Kollision.
    // Sie ist einfach nur da. Daher kann diese Funktion leer bleiben.
    // Die Logik (z.B. "Spieler wird gestoppt") passiert im
    // onCollision des *anderen* Objekts (z.B. im Spieler).
}