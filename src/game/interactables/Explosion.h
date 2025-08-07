#pragma once
#include "../Collidable.h"

class Explosion : public Collidable
{
private:
    float lifetime;
    // Hier können später Animationen hinzugefügt werden

public:
    Explosion(Vector2 position);
    ~Explosion() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;
};