#pragma once

#include <map>
#include "../EnemySpawner.h"
#include <raylib.h>
#include "../../core/Object_Manager.h"
#include "../enemys/enemies_list.h"
#include <memory>
#include <vector>

class InfinitySpawner : public Enemy_Spawner
{
public:
    InfinitySpawner(Rectangle spawner_Area, Object_Manager& om);
    ~InfinitySpawner() override;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;

private:
    Texture2D spawner_sprite;
    Object_Manager& object_manager_ref;
    float spawn_timer;
    int max_enemies_per_instance_;
    std::vector<std::weak_ptr<enemy::Enemy_Base_Class>> spawned_enemies_;

    void Update_And_Count_Spawned_Enemies(std::map<enemy::EnemyType, int>& counts);
    enemy::Enemy_Base_Class* create_enemy_by_type(Vector2 position, enemy::EnemyType type);

protected:
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override;
};