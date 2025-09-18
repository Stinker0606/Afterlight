#pragma once
#include "../Collidable.h"
#include "../../core/ControllableAnimations.h"

class KELPIE : public Collidable
{
private:
    Texture2D dia_trigger;
    ControllableAnimations animation_;

public:
    KELPIE(Vector2 position);
    ~KELPIE() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};