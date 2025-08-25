#pragma once

#include "../Collidable.h"
#include <raylib.h>

#include "Object_Manager.h"

namespace game::scenes
{
    class Level1Scene;
}

class Bomb : public Collidable
{
private:
    Texture2D texture; // Bomben haben ihre eigene Textur
    float detonation_timer;
    float blink_timer;
    bool is_visible;
    game::scenes::Level1Scene* scene_context; // Wichtig, um die Explosion zu spawnen
    Object_Manager& om_ref_; // Eine Referenz zum Object Manager

public:
    Bomb(Vector2 position, game::scenes::Level1Scene* scene, Object_Manager& om);
    ~Bomb() override;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

private:
    void Detonate();
};