#pragma once
#include "Object_Manager.h"
#include "../Collidable.h"
#include "../../core/RepeatAnimation.h"

class Explosion : public Collidable
{
private:
    float lifetime;
    Object_Manager& om_ref_;
    RepeatAnimation animation_;

public:
    Explosion(Vector2 position, Object_Manager& om);
    ~Explosion() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};