#include "MimicSpawner.h"
#include "../enemys/enemies_list.h"
#include "../../config_enemies.h.in"
#include "../CollisionResponse.h"
#include "AssetManager.h"
#include "Store.h"
#include "raymath.h"
#include "../enemys/Mimic.h"

MimicSpawner::MimicSpawner(Rectangle spawner_Area, Object_Manager& om)
  : Enemy_Spawner(spawner_Area, {}, nullptr, 0.0f, 0, om),
    is_triggered_(false)
{
    // Lade das statische Sprite aus der neuen Config-Variable
    this->statue_sprite_ = AssetManager::GetInstance().Load(game::EnemyConfig::kMimicStatueSpritePath);

    this->hitbox = { spawner_Area.x, spawner_Area.y-32, 32.0f, 64.0f };
    this->useFog = true;
}

void MimicSpawner::Tick(float delta_time)
{
    if (is_triggered_) return;

    if (game::core::Store::player)
    {
        Vector2 player_pos = game::core::Store::player->Get_Hitbox_Center();


        if (Vector2Distance(this->Get_Hitbox_Center(), player_pos) <= game::EnemyConfig::kMimicAggroRadius)
        {
            is_triggered_ = true;

            Vector2 mimic_spawn_pos = { this->hitbox.x, this->hitbox.y +32};
            auto mimic_enemy = std::make_shared<enemy::Mimic>(mimic_spawn_pos, om_ref_);

            om_ref_.AddObject(mimic_enemy);

            this->Mark_For_Destruction();
        }
    }
}

void MimicSpawner::Draw()
{
    Vector2 draw_pos = {
        // ÄNDERUNG HIER: Verwende den neuen Spawner-Offset
        this->hitbox.x - game::EnemyConfig::kMimicSpawner_visual_offset.x,
        this->hitbox.y - game::EnemyConfig::kMimicSpawner_visual_offset.y
    };
    DrawTextureV(this->statue_sprite_, draw_pos, Fade(WHITE, this->visibility_alpha));
}


void MimicSpawner::On_Collision(std::shared_ptr<Collidable> other)
{
    if (other->Get_Collision_Type() == Collision_Type::PLAYER)
    {
        CollisionResponse::Resolve_Overlap(other, shared_from_this());
    }
}