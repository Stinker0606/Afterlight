#pragma once

#include "../EnemySpawner.h"
#include <memory>

class Object_Manager;

class MimicSpawner : public Enemy_Spawner
{
public:
    MimicSpawner(Rectangle spawner_Area, Object_Manager& om);
    ~MimicSpawner() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;

private:
    Texture2D statue_sprite_;
    bool is_triggered_;

protected:
    // Bleibt leer, da dieser Spawner eine spezielle Logik hat
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override { return nullptr; }
};