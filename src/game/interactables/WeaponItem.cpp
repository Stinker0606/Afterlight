#include "WeaponItem.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../../config.h.in"

WeaponItem::WeaponItem(Vector2 position, int weapon_id, Texture2D tileset, Rectangle source_rect)
    : weapon_id_(weapon_id), tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, source_rect.width, source_rect.height };
}

void WeaponItem::Tick(float delta_time) {
    // Wenn der Spieler sich entfernt, deaktiviere die Interaktionsmöglichkeit
    is_in_range_ = false;
}

void WeaponItem::Draw() {
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
    if (is_in_range_) {
        // Zeichne ein "E" über dem Item, um die Interaktion anzuzeigen
        DrawText("E", hitbox.x + 8, hitbox.y - 20, 20, YELLOW);
    }
}

void WeaponItem::On_Collision(std::shared_ptr<Collidable> other) {
    if (auto player = std::dynamic_pointer_cast<PlayerClass>(other)) {
        is_in_range_ = true; // Spieler ist in der Nähe
        if (IsKeyPressed(game::Config::key_Interact) && player->GetHeldWeapon() == -1) {
            player->SetHeldWeapon(this->weapon_id_);
            SoundManager::GetInstance().PlaySfx("item_pickup_key"); // Placeholder-Sound
            this->Mark_For_Destruction();
        }
    }
}

Collision_Type WeaponItem::Get_Collision_Type() const {
    return Collision_Type::NONE; // Kein solides Objekt, man kann darüber laufen
}

int WeaponItem::GetWeaponID() const {
    return weapon_id_;
}