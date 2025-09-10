#include "Level3Spawner.h"
#include "../enemys/enemies_list.h"
#include "../../config_enemies.h.in"
#include "AssetManager.h"

Level3_Spawner::Level3_Spawner(Rectangle spawner_Area,
                               const std::vector<Rectangle>& obstacle_List,
                               std::vector<enemy::Enemy_Base_Class*>* enemy_List,
                               float spawn_Rate,
                               int max_Enemies,
                               Object_Manager& om)
  : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List,
                  game::EnemyConfig::kSpawner3_SpawnRate, // Korrigiert auf Spawner 3
                  game::EnemyConfig::kSpawner3_MaxEnemies,  // Korrigiert auf Spawner 3
                  om),
    object_manager_ref(om),
    spawn_timer(game::EnemyConfig::kSpawner3_SpawnRate), // Korrigiert auf Spawner 3
    max_enemies_per_instance_(game::EnemyConfig::kSpawner3_MaxEnemies), // Korrigiert auf Spawner 3
    health_(1)
{
    this->hitbox = spawner_Area;
    this->spawner_sprite = AssetManager::GetInstance().Load("assets/graphics/spawner/corpse_Knight_spawner.png");
}

Level3_Spawner::~Level3_Spawner() {}

void Level3_Spawner::Update_And_Count_Spawned_Enemies(std::map<enemy::EnemyType, int>& counts)
{
    counts.clear();
    spawned_enemies_.erase(std::remove_if(spawned_enemies_.begin(), spawned_enemies_.end(),
        [&counts](const std::weak_ptr<enemy::Enemy_Base_Class>& e) {
            if (auto locked_enemy = e.lock()) {
                if (std::dynamic_pointer_cast<enemy::Corpse>(locked_enemy)) {
                    counts[enemy::EnemyType::CORPSE]++;
                }
                return false;
            }
            return true;
        }), spawned_enemies_.end());
}

void Level3_Spawner::Tick(float delta_time)
{
    if (health_ <= 0) return;
    spawn_timer -= delta_time;

    if (spawn_timer <= 0.0f)
    {
        spawn_timer = spawn_Rate_;
        std::map<enemy::EnemyType, int> on_field_counts;
        Update_And_Count_Spawned_Enemies(on_field_counts);

        if (spawned_enemies_.size() < max_enemies_per_instance_)
        {
            std::vector<SpawnableEnemy> possible_spawns;
            int total_weight = 0;
            for (const auto& spawn_info : game::EnemyConfig::kSpawner3_EnemyPool) // Korrigiert auf Spawner 3
            {
                int current_count = on_field_counts.count(spawn_info.type) ? on_field_counts[spawn_info.type] : 0;
                if (current_count < spawn_info.max_on_field) {
                    possible_spawns.push_back(spawn_info);
                    total_weight += spawn_info.weight;
                }
            }
            if (!possible_spawns.empty())
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

                        enemy::Enemy_Base_Class* new_enemy_raw = create_enemy_by_type(spawn_pos, spawn_info.type);
                        if (new_enemy_raw) {
                            auto new_enemy_shared = std::shared_ptr<enemy::Enemy_Base_Class>(new_enemy_raw);
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

void Level3_Spawner::Draw()
{
    DrawTextureV(this->spawner_sprite, {(float)this->hitbox.x, (float)this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

void Level3_Spawner::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER_PROJECTILE || other->Get_Collision_Type() == Collision_Type::PLAYER_MELEE_HITBOX)
    {
        this->health_--;
        if (health_ <= 0)
        {
            this->Mark_For_Destruction();
        }
    }
}

enemy::Enemy_Base_Class* Level3_Spawner::create_enemy_by_type(Vector2 position, enemy::EnemyType type)
{
    switch (type) {
        case enemy::EnemyType::CORPSE:
            return new enemy::Corpse(position, object_manager_ref, false);
        default:
            return nullptr;
    }
}

// Diese Funktion bleibt leer, da die Logik in create_enemy_by_type liegt
enemy::Enemy_Base_Class* Level3_Spawner::createEnemy(Vector2 position)
{
    return nullptr;
}