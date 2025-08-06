#include "PushBlock.h"
#include "../PlayerBaseClass.h"

Push_Block::Push_Block(Vector2 position, Texture2D tileset, Rectangle source_rect)
    : tileset_texture(tileset), texture_source_rect(source_rect)
{
    this->hitbox = { position.x, position.y, 32.0f, 32.0f };
    this->previous_position = { position.x, position.y };
}

void Push_Block::Tick(float delta_time)
{
    // Speichere die Position zu Beginn des Frames
    this->previous_position = { hitbox.x, hitbox.y };
}

void Push_Block::Draw()
{
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

void Push_Block::On_Collision(std::shared_ptr<Collidable> other)
{
    // Wenn der Block mit einer Wand oder einem anderen schiebbaren Block kollidiert,
    // wird er auf seine vorherige Position zurückgesetzt.
    auto other_type = other->Get_Collision_Type();
    if (other_type == Collision_Type::WALL || other_type == Collision_Type::ENEMY_SPAWNER ||
        (other.get() != this && other->Get_Collision_Type() == Get_Collision_Type())) // Verhindert Selbstkollision
    {
        this->hitbox.x = this->previous_position.x;
        this->hitbox.y = this->previous_position.y;
    }
}

Collision_Type Push_Block::Get_Collision_Type() const
{
    // Wir geben ihm einen eigenen Typ, um ihn von normalen Wänden zu unterscheiden,
    // auch wenn er sich vorerst wie eine verhält.
    return Collision_Type::WALL;
}

void Push_Block::Push(Vector2 move_direction)
{
    // Bewege den Block um eine ganze Kachel (32 Pixel) in die gegebene Richtung.
    // Die Bewegung ist "Tile-basiert", nicht flüssig.
    this->hitbox.x += move_direction.x * 32.0f;
    this->hitbox.y += move_direction.y * 32.0f;
}