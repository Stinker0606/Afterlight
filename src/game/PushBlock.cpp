#include "PushBlock.h"

Push_Block::Push_Block(Vector2 position, Texture2D tileset, Rectangle source_rect)
    : tileset_texture(tileset), texture_source_rect(source_rect)
{
    // Die Hitbox des Blocks ist 32x32, genau wie eine Kachel.
    // Wir zentrieren die Hitbox auf die gezeichnete Kachel.
    this->hitbox = { position.x, position.y, 32.0f, 32.0f };
}

// Die Logik für das Schieben kommt später hier rein.
void Push_Block::Tick(float delta_time)
{
    // Vorerst tut der Block nichts.
}

// Hier wird die spezifische Kachel aus dem Tileset gezeichnet.
void Push_Block::Draw()
{
    // Wir benutzen die `visibility_alpha`-Eigenschaft, die jedes Collidable jetzt hat,
    // damit der Block später auch vom Nebel betroffen sein kann.
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

// Die Kollisionslogik kommt später hier rein.
void Push_Block::On_Collision(std::shared_ptr<Collidable> other)
{
    // Vorerst passiert nichts bei Kollision.
}

Collision_Type Push_Block::Get_Collision_Type() const
{
    // Wir geben ihm vorerst den Typ WALL, damit der Spieler nicht durchlaufen kann.
    return Collision_Type::WALL;
}