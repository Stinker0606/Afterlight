#pragma once
#include "../Collidable.h"

class Statue : public Collidable
{
private:
    int correct_weapon_id_;
    Texture2D sprite_with_weapon_;
    Texture2D sprite_no_weapon_;
    Texture2D tileset_texture;
    Rectangle texture_source_rect;
    Texture2D statue;

    bool is_solved_ = false;
    bool is_in_range_ = false;

public:
    Statue(Vector2 position, int correct_weapon_id, Texture2D tex_with_weapon, Texture2D tex_no_weapon);
    ~Statue() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    bool IsSolved() const;
};