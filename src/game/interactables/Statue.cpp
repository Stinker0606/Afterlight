#include "Statue.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"

Statue::Statue(Vector2 position, int correct_weapon_id, Texture2D tex_with_weapon, Texture2D tex_no_weapon)
    : correct_weapon_id_(correct_weapon_id),
      sprite_with_weapon_(tex_with_weapon),
      sprite_no_weapon_(tex_no_weapon)
{
    hitbox = { position.x, position.y, (float)sprite_no_weapon_.width, (float)sprite_no_weapon_.height };
    statue = AssetManager::GetInstance().Load("assets/graphics/ui/E_DiaTrig.png");
}

void Statue::Tick(float delta_time) {
    is_in_range_ = false;
}

void Statue::Draw() {
    if (is_solved_) {
        float draw_y = (hitbox.y + hitbox.height) - sprite_with_weapon_.height;
        DrawTexture(sprite_with_weapon_, hitbox.x, draw_y, Fade(WHITE, visibility_alpha));
    } else {
        DrawTexture(sprite_no_weapon_, hitbox.x, hitbox.y, Fade(WHITE, visibility_alpha));
    }

    if (is_in_range_ && !is_solved_) {
        DrawTextureV(statue,{hitbox.x + hitbox.width / 2, hitbox.y - 20}, WHITE);
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
                SoundManager::GetInstance().PlaySfx("keywall_oen"); // Placeholder-Sound
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