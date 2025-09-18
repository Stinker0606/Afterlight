#include "BreakableWall.h"
#include "Explosion.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

BreakableWall::BreakableWall(Vector2 position, Texture2D tileset, Rectangle source_rect)
    : tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, 32.0f, 32.0f };
}

void BreakableWall::Tick(float delta_time) {}

void BreakableWall::Draw()
{
    DrawTextureRec(tileset_texture, texture_source_rect, { hitbox.x, hitbox.y }, Fade(WHITE, visibility_alpha));
}

void BreakableWall::DestroyWall()
{
    // Nur einmal ausführen
    if (Is_Marked_For_Destruction()) return;

    SoundManager::GetInstance().PlaySfx("wall_break", 1);
    this->Mark_For_Destruction();
}

void BreakableWall::On_Collision(std::shared_ptr<Collidable> other)
{
    // Die Wand reagiert jetzt nur noch, wenn sie von einer Explosion getroffen wird.
    if (std::dynamic_pointer_cast<Explosion>(other))
    {
        DestroyWall();
    }
}

Collision_Type BreakableWall::Get_Collision_Type() const
{
    return Collision_Type::WALL;
}