//
// Created by Kruse on 02/06/2025.
//

#include "Walls.h"
#include "CollisionManager.h"

Walls::Walls(Vector2 position, Vector2 size)
{
    hitbox = { position.x, position.y, size.x, size.y };
}

Walls::~Walls()
{
}

Collision_Type Walls::Get_Collision_Type() const
{
    return Collision_Type::WALL;
}

void Walls::Tick(float delta_time) {
    // Wände bewegen sich nicht.
}

void Walls::On_Collision(Collidable* other)
{
    // Die Reaktion wird vom Spieler gehandhabt.
}

// KORREKTUR: Wir fügen hier die Zeichenlogik hinzu.
void Walls::Draw()
{
    // Zeichnet die Hitbox der Wand als durchsichtiges Rechteck.
    // So siehst du die Kollisionsgrenzen, ohne die Tilemap zu verdecken.
    // Wenn du sie unsichtbar machen willst, ändere die letzte Zahl (Alpha) auf 0.
    DrawRectangleRec(this->hitbox, { 100, 100, 100, 0 });
}