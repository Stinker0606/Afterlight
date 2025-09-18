#pragma once
#include "../Collidable.h"

class PlayerClass;

class WeaponItem : public Collidable
{
private:
    Texture2D tileset_texture;
    Texture2D dia_trigger;
    Rectangle texture_source_rect;
    int weapon_id_;
    bool is_in_range_ = false;

public:
    WeaponItem(Vector2 position, int weapon_id, Texture2D tileset, Rectangle source_rect);
    ~WeaponItem() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    int GetWeaponID() const;
};