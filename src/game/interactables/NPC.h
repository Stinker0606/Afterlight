#pragma once
#include "../Collidable.h"
#include "../../core/ControllableAnimations.h"

class NPC : public Collidable
{
private:
    Texture2D dia_trigger;
    ControllableAnimations animation_;
    bool is_in_range_ = false;
    bool has_given_key_ = false;

public:
    NPC(Vector2 position);
    ~NPC() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};