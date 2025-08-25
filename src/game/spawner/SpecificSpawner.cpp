#include "SpecificSpawner.h"

#include "AssetManager.h"
#include "../../config_enemies.h.in"

SpecificSpawner::SpecificSpawner(Rectangle spawner_Area, float spawn_Rate, int max_Enemies,
                                 Object_Manager& om, enemy::EnemyType enemy_type)
    : Enemy_Spawner(spawner_Area, {}, nullptr, spawn_Rate, max_Enemies, om),
      type_to_spawn_(enemy_type),
      spawn_timer_(spawn_Rate),
      health_(1)
{
    this->hitbox = spawner_Area;
    this->spawner_sprite_ = AssetManager::GetInstance().Load("assets/graphics/spawner/Spawner_outside.png");
    this->useFog = true;
}

SpecificSpawner::~SpecificSpawner()
{
    /*if (spawner_sprite_.id > 0)
    {
        UnloadTexture(spawner_sprite_);
    }*/
}

void SpecificSpawner::Tick(float delta_time)
{
    if (health_ <= 0) return;
    spawn_timer_ -= delta_time;

    // Bereinige tote Gegner aus der Liste
    spawned_enemies_.erase(std::remove_if(spawned_enemies_.begin(), spawned_enemies_.end(),
        [](const std::weak_ptr<enemy::Enemy_Base_Class>& e) {
            return e.expired();
        }), spawned_enemies_.end());

    if (spawn_timer_ <= 0.0f && spawned_enemies_.size() < max_Enemies_)
    {
        spawn_timer_ = spawn_Rate_;
        Vector2 spawn_pos = {
            hitbox.x + GetRandomValue(0, (int)hitbox.width - 32),
            hitbox.y + GetRandomValue(0, (int)hitbox.height - 32)
        };

        enemy::Enemy_Base_Class* new_enemy_raw = createEnemy(spawn_pos);

        if (new_enemy_raw) {
            auto new_enemy_shared = std::shared_ptr<enemy::Enemy_Base_Class>(new_enemy_raw);
            om_ref_.AddObject(new_enemy_shared);
            spawned_enemies_.push_back(new_enemy_shared);
        }
    }
}

void SpecificSpawner::Draw()
{
    DrawTextureV(this->spawner_sprite_, {this->hitbox.x, this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

void SpecificSpawner::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER_PROJECTILE) {
        this->health_--;
        if (health_ <= 0) {
            this->Mark_For_Destruction();
        }
    }
}

enemy::Enemy_Base_Class* SpecificSpawner::createEnemy(Vector2 position)
{
    // Erschaffe den spezifischen Gegner
    switch (type_to_spawn_)
    {
        case enemy::EnemyType::INSECT_MONSTER:
            return new enemy::Insect_Monster(position, om_ref_, true);
        case enemy::EnemyType::DROWNED_SNIPER:
            return new enemy::DrownedSniper(position, om_ref_, true);
    }
    return nullptr;
}