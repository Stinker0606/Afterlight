#include "Walls.h"
#include "CollisionManager.h"

Walls::Walls(Vector2 position, Vector2 size)
{
    hitbox = { position.x, position.y, size.x, size.y };
}

Walls::~Walls() {}

Collision_Type Walls::Get_Collision_Type() const {
    return Collision_Type::WALL;
}

void Walls::Tick(float delta_time) {}
void Walls::On_Collision(Collidable* other) {}

void Walls::Draw()
{
    // Zeichnet die Hitbox der Wand als durchsichtiges Rechteck.
    // So sind die Kollisionsgrenzen sichtbar, ohne die Welt zu überdecken.
    DrawRectangleRec(this->hitbox, { 100, 100, 100, 0 });
}