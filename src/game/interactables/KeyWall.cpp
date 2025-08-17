#include "KeyWall.h"
#include "../PlayerClass.h"

KeyWall::KeyWall(Vector2 position, Texture2D tileset, Rectangle source_rect)
    : tileset_texture(tileset), texture_source_rect(source_rect)
{
    this->hitbox = { position.x, position.y, 32.0f, 32.0f };
}

// Die Draw-Methode zeichnet die Kachel aus dem Tileset
void KeyWall::Draw()
{
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

Collision_Type KeyWall::Get_Collision_Type() const
{
    // Sie verhält sich wie eine normale Wand, solange sie existiert.
    return Collision_Type::WALL;
}

void KeyWall::On_Collision(std::shared_ptr<Collidable> other)
{

}