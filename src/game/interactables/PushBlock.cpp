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
    // Kollisionen mit dem Spieler werden von der Player-Klasse gehandhabt,
    // hier also ignorieren, um Zyklen zu vermeiden.
    if (other->Get_Collision_Type() == Collision_Type::PLAYER) return;

    // Prüfe, ob das andere Objekt "solide" ist.
    // Ein Push_Block gibt selbst den Typ WALL zurück, also wird der erste Check
    // auch die Kollision mit anderen Push_Blocks abfangen.
    if (other->Get_Collision_Type() == Collision_Type::WALL ||
        other->Get_Collision_Type() == Collision_Type::ENEMY_SPAWNER ||
        other->Get_Collision_Type() == Collision_Type::ENEMY)
    {
        // Setze die Position auf die Position VOR dem Push zurück.
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