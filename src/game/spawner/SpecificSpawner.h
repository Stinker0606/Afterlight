#pragma once
#include "../EnemySpawner.h"
#include "../enemys/enemies_list.h"
#include <memory>
#include <vector>

// Ein universeller Spawner, der einen spezifischen Gegnertyp erzeugt
// und sich dabei wie ein vollwertiges Spielobjekt verhält.
class SpecificSpawner : public Enemy_Spawner
{
public:
    SpecificSpawner(Rectangle spawner_Area, float spawn_Rate, int max_Enemies,
                    Object_Manager& om, enemy::EnemyType enemy_type);
    ~SpecificSpawner() override;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;

private:
    Texture2D spawner_sprite_;
    enemy::EnemyType type_to_spawn_;
    float spawn_timer_;
    int health_;

    // Behält den Überblick über die von diesem Spawner erstellten Gegner
    std::vector<std::weak_ptr<enemy::Enemy_Base_Class>> spawned_enemies_;

protected:
    // Implementiert die reine Erschaffungs-Logik.
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override;
};