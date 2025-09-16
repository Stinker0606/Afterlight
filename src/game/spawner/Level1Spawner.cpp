#include "Level1Spawner.h"
#include "../enemys/enemies_list.h"
#include "../../config_enemies.h.in"
#include "../PlayerProjectile.h"
#include "AssetManager.h"

Level1_Spawner::Level1_Spawner(Rectangle spawner_Area,
                               const std::vector<Rectangle>& obstacle_List,
                               std::vector<enemy::Enemy_Base_Class*>* enemy_List,
                               float spawn_Rate,
                               int max_Enemies,
                               Object_Manager& om)
  // Wir rufen den Basis-Konstruktor wieder mit den festen Werten aus der Config auf.
  : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List,
                  game::EnemyConfig::kSpawner1_SpawnRate,
                  game::EnemyConfig::kSpawner1_MaxEnemies,
                  om),
    object_manager_ref(om),
      spawn_timer(game::EnemyConfig::kSpawner1_SpawnRate),
      max_enemies_per_instance_(game::EnemyConfig::kSpawner1_MaxEnemies),
      health_(1)
{
    this->hitbox = spawner_Area;
    this->spawner_sprite = AssetManager::GetInstance().Load("assets/graphics/spawner/Spawner_outside.png");
}

Level1_Spawner::~Level1_Spawner()
{
   /* if (spawner_sprite.id > 0)
    {
        UnloadTexture(this->spawner_sprite);
    }*/
}

void Level1_Spawner::Update_And_Count_Spawned_Enemies(std::map<enemy::EnemyType, int>& counts)
{
    counts.clear();
    spawned_enemies_.erase(std::remove_if(spawned_enemies_.begin(), spawned_enemies_.end(),
        [&counts](const std::weak_ptr<enemy::Enemy_Base_Class>& e) {
            if (auto locked_enemy = e.lock()) {
                // Zähle die Typen der lebenden Gegner
                if (auto insect = std::dynamic_pointer_cast<enemy::Insect_Monster>(locked_enemy)) {
                    counts[enemy::EnemyType::INSECT_MONSTER]++;
                } else if (std::dynamic_pointer_cast<enemy::DarknessMonster>(locked_enemy)) {
                    counts[enemy::EnemyType::DARKNESS_MONSTER]++;
                }
                // Hier weitere else-if für andere Gegnertypen
                return false; // Behalte in der Liste
            }
            return true; // Entferne aus der Liste, da abgelaufen
        }), spawned_enemies_.end());
}

void Level1_Spawner::Tick(float delta_time)
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
            for (const auto& spawn_info : game::EnemyConfig::kSpawner1_EnemyPool)
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

                        // Hier wird die Logik zur Gegnererstellung direkt aufgerufen
                        enemy::Enemy_Base_Class* new_enemy_raw = nullptr;
                        switch (spawn_info.type) {
                            case enemy::EnemyType::INSECT_MONSTER:
                                // Übergebe die object_manager_ref an den Konstruktor
                                    new_enemy_raw = new enemy::Insect_Monster(spawn_pos, object_manager_ref, true);
                            break;
                            case enemy::EnemyType::DARKNESS_MONSTER:
                                new_enemy_raw = new enemy::DarknessMonster(spawn_pos, object_manager_ref, true);
                            break;
                            // Hier weitere cases für andere Gegner
                        }

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

void Level1_Spawner::Draw()
{
    // Zeichne das Spawner-Sprite mit der korrekten Transparenz für den Nebel.
    DrawTextureV(this->spawner_sprite, {(float)this->hitbox.x, (float)this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

// On_Collision Implementierung
void Level1_Spawner::On_Collision(std::shared_ptr<Collidable> other)
{
    // Spawner reagieren NUR auf Spieler-Projektile
    if (other->Get_Collision_Type() == Collision_Type::PLAYER_PROJECTILE || other->Get_Collision_Type() == Collision_Type::PLAYER_MELEE_HITBOX)
    {
        // Wir prüfen nicht, ob es eine Bombe oder ein Speer ist, sondern einfach, ob es ein Spieler-Angriff ist.
        this->health_--;
        if (health_ <= 0)
        {
            this->Mark_For_Destruction();
        }
    }
}

enemy::Enemy_Base_Class* Level1_Spawner::createEnemy(Vector2 position)
{
    return new enemy::Insect_Monster(position, object_manager_ref,true);
}