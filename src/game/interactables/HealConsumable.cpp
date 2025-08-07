#include "HealConsumable.h"
#include "../PlayerBaseClass.h"

HealConsumable::HealConsumable(Vector2 position, int healAmount, Texture2D tileset, Rectangle source_rect)
    : heal_amount(healAmount), tileset_texture(tileset), texture_source_rect(source_rect)
{
    hitbox = { position.x, position.y, 32.0f, 32.0f };
}

HealConsumable::~HealConsumable() {}

void HealConsumable::Tick(float delta_time)
{
    // Items bewegen sich nicht.
}

void HealConsumable::Draw()
{
    // Zeichne das Item mit den korrekten Grafik-Daten aus dem Tileset.
    DrawTextureRec(this->tileset_texture, this->texture_source_rect, { this->hitbox.x, this->hitbox.y }, Fade(WHITE, this->visibility_alpha));
}

void HealConsumable::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        if (auto player = std::dynamic_pointer_cast<Player_Base_Class>(other))
        {
            ApplyEffect(player.get());
            this->Mark_For_Destruction();
        }
    }
}

Collision_Type HealConsumable::Get_Collision_Type() const
{
    return Collision_Type::CONSUMABLE;
}

void HealConsumable::ApplyEffect(Player_Base_Class* player)
{
    player->Take_Damage(-this->heal_amount);
}