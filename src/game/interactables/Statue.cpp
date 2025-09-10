#include "Statue.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"

Statue::Statue(Vector2 position, int correct_weapon_id, Texture2D tex_with_weapon, Texture2D tileset, Rectangle source_rect)
    : correct_weapon_id_(correct_weapon_id), sprite_with_weapon_(tex_with_weapon), tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, source_rect.width, source_rect.height };
}

void Statue::Tick(float delta_time) {
    is_in_range_ = false;
}

void Statue::Draw() {
    if (is_solved_) {
        DrawTexture(sprite_with_weapon_, hitbox.x, hitbox.y, Fade(WHITE, visibility_alpha));
    } else {
        DrawTextureRec(tileset_texture, texture_source_rect, { hitbox.x, hitbox.y }, Fade(WHITE, visibility_alpha));
    }

    if (is_in_range_ && !is_solved_) {
        DrawText("E", hitbox.x + hitbox.width / 2 - 10, hitbox.y - 20, 20, Color { 216, 176, 168, 255 });
    }
}

void Statue::On_Collision(std::shared_ptr<Collidable> other) {
    if (is_solved_) return;

    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        is_in_range_ = true;
        if (IsKeyPressed(game::Config::key_Interact)) {
            if (player->GetHeldWeapon() == correct_weapon_id_) {
                is_solved_ = true;
                player->ClearHeldWeapon();
                SoundManager::GetInstance().PlaySfx("keywall_open"); // Placeholder-Sound
            }
        }
    }
}

Collision_Type Statue::Get_Collision_Type() const {
    return Collision_Type::WALL; // Statuen sind solide Hindernisse
}

bool Statue::IsSolved() const {
    return is_solved_;
}