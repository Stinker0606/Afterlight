#include "BombConsumable.h"
#include "../PlayerClass.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

BombConsumable::BombConsumable(Vector2 position, int amount, Texture2D tileset, Rectangle source_rect)
    : bomb_amount(amount), tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, 32.0f, 32.0f };
    useFog = true;
}

void BombConsumable::Tick(float delta_time) {}

void BombConsumable::Draw()
{
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

void BombConsumable::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        SoundManager::GetInstance().PlaySfx("item_pickup_bomb");

        if (auto player = std::dynamic_pointer_cast<PlayerClass>(other))
        {
            player->Add_Bomb(this->bomb_amount);
            this->Mark_For_Destruction();
        }
    }
}

Collision_Type BombConsumable::Get_Collision_Type() const
{
    return Collision_Type::CONSUMABLE;
}