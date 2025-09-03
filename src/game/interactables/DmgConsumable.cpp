#include "DmgConsumable.h"
#include "../PlayerBaseClass.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

DmgConsumable::DmgConsumable(Vector2 position, int damageAmount, Texture2D tileset, Rectangle source_rect)
    : damage_amount(damageAmount), tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, 32.0f, 32.0f };
}

DmgConsumable::~DmgConsumable() {}

void DmgConsumable::Tick(float delta_time)
{
    // Items bewegen sich nicht.
}

void DmgConsumable::Draw()
{
    // Zeichne das Item mit den korrekten Grafik-Daten aus dem Tileset.
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

void DmgConsumable::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        SoundManager::GetInstance().PlaySfx("item_pickup_damage");

        if (auto player = std::dynamic_pointer_cast<Player_Base_Class>(other))
        {
            ApplyEffect(player.get());
            this->Mark_For_Destruction();
        }
    }
}

Collision_Type DmgConsumable::Get_Collision_Type() const
{
    return Collision_Type::CONSUMABLE;
}

void DmgConsumable::ApplyEffect(Player_Base_Class* player)
{
    player->Take_Damage(this->damage_amount);
}