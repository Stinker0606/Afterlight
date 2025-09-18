#include "InfinitySpawner.h"
#include "../enemys/enemies_list.h"
#include "../../config_enemies.h.in"
#include "AssetManager.h"

InfinitySpawner::InfinitySpawner(Rectangle spawner_Area, Object_Manager& om)
  : Enemy_Spawner(spawner_Area, {}, nullptr,
                  game::EnemyConfig::kInfinitySpawner_SpawnRate,
                  game::EnemyConfig::kInfinitySpawner_MaxEnemies,
                  om),
    object_manager_ref(om),
    spawn_timer(game::EnemyConfig::kInfinitySpawner_SpawnRate),
    max_enemies_per_instance_(game::EnemyConfig::kInfinitySpawner_MaxEnemies)
{
    this->hitbox = spawner_Area;
    this->spawner_sprite = AssetManager::GetInstance().Load("assets/graphics/spawner/corpse_Knight_spawner.png");
}

InfinitySpawner::~InfinitySpawner() {}

void InfinitySpawner::Update_And_Count_Spawned_Enemies(std::map<enemy::EnemyType, int>& counts)
{
    counts.clear();
    spawned_enemies_.erase(std::remove_if(spawned_enemies_.begin(), spawned_enemies_.end(),
        [&counts](const std::weak_ptr<enemy::Enemy_Base_Class>& e) {
            if (auto locked_enemy = e.lock()) {
                if (std::dynamic_pointer_cast<enemy::Insect_Monster>(locked_enemy)) counts[enemy::EnemyType::INSECT_MONSTER]++;
                else if (std::dynamic_pointer_cast<enemy::WalkingCorpse>(locked_enemy)) counts[enemy::EnemyType::WALKING_CORPSE]++;
                else if (std::dynamic_pointer_cast<enemy::Corpse>(locked_enemy)) counts[enemy::EnemyType::CORPSE]++;
                else if (std::dynamic_pointer_cast<enemy::Mimic>(locked_enemy)) counts[enemy::EnemyType::MIMIC]++;
                else if (std::dynamic_pointer_cast<enemy::DarknessMonster>(locked_enemy)) counts[enemy::EnemyType::DARKNESS_MONSTER]++;
                return false;
            }
            return true;
        }), spawned_enemies_.end());
}

void InfinitySpawner::Tick(float delta_time)
{
    spawn_timer -= delta_time;

    if (spawn_timer <= 0.0f)
    {
        spawn_timer = spawn_Rate_;
        std::map<enemy::EnemyType, int> on_field_counts;
        Update_And_Count_Spawned_Enemies(on_field_counts);

        if (spawned_enemies_.size() < max_enemies_per_instance_)
        {
            const auto& enemy_pool = game::EnemyConfig::kInfinitySpawner_EnemyPool;
            if (enemy_pool.empty()) return;

            std::vector<SpawnableEnemy> possible_spawns;
            int total_weight = 0;

            for (const auto& spawn_info : enemy_pool)
            {
                int current_count = on_field_counts.count(spawn_info.type) ? on_field_counts[spawn_info.type] : 0;
                if (current_count < spawn_info.max_on_field) {
                    possible_spawns.push_back(spawn_info);
                    total_weight += spawn_info.weight;
                }
            }

            if (!possible_spawns.empty() && total_weight > 0) // KORREKTUR: Fügt die total_weight > 0 Prüfung hinzu
            {
                int random_value = GetRandomValue(0, total_weight - 1);
                int cumulative_weight = 0;
                for (const auto& spawn_info : possible_spawns)
                {
                    cumulative_weight += spawn_info.weight;
                    if (random_value < cumulative_weight)
                    {
                        Vector2 spawn_pos = {
                            hitbox.x + GetRandomValue(0, (int)hitbox.width - 32),
                            hitbox.y + GetRandomValue(0, (int)hitbox.height - 32)
                        };

                        if (auto new_enemy_shared = std::shared_ptr<enemy::Enemy_Base_Class>(create_enemy_by_type(spawn_pos, spawn_info.type))) {
                            object_manager_ref.AddObject(new_enemy_shared);
                            spawned_enemies_.push_back(new_enemy_shared);
                        }
                        break;
                    }
                }
            }
        }
    }
}

void InfinitySpawner::Draw()
{
    DrawTextureV(this->spawner_sprite, {(float)this->hitbox.x, (float)this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

void InfinitySpawner::On_Collision(std::shared_ptr<Collidable> other)
{
    // Dieser Spawner ist unzerstörbar und reagiert nicht auf Kollisionen.
}

enemy::Enemy_Base_Class* InfinitySpawner::create_enemy_by_type(Vector2 position, enemy::EnemyType type)
{
    switch (type) {
        case enemy::EnemyType::INSECT_MONSTER:      return new enemy::Insect_Monster(position, object_manager_ref, true);
        case enemy::EnemyType::WALKING_CORPSE:      return new enemy::WalkingCorpse(position, object_manager_ref, true);
        case enemy::EnemyType::CORPSE:              return new enemy::Corpse(position, object_manager_ref, false);
        case enemy::EnemyType::MIMIC:               return new enemy::Mimic(position, object_manager_ref, true);
        case enemy::EnemyType::DARKNESS_MONSTER:    return new enemy::DarknessMonster(position, object_manager_ref, true);
        default:                                    return nullptr;
    }
}

enemy::Enemy_Base_Class* InfinitySpawner::createEnemy(Vector2 position)
{
    return nullptr; // Wird nicht direkt verwendet, die Logik liegt in create_enemy_by_type
}