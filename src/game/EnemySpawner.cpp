#include "EnemySpawner.h"
#include "raylib.h"
#include "CollisionManager.h"

Enemy_Spawner::Enemy_Spawner(Rectangle spawner_Area, const std::vector<Rectangle>& obstacle_List,
                 std::vector<enemy::Enemy_Base_Class*>& enemy_List, float spawn_Rate, int max_Enemies)
    : spawner_Area(spawner_Area), obstacle_List(obstacle_List), enemy_List(enemy_List),  spawn_Rate_(spawn_Rate),
      max_Enemies_(max_Enemies), time_Since_Last_Spawn_(0.0f) {}

void Enemy_Spawner::Update(float delta_Time)
{
    time_Since_Last_Spawn_ += delta_Time;

    if (time_Since_Last_Spawn_ >= (1.0f / spawn_Rate_) &&
        (int)enemy_List.size() < max_Enemies_) {

        Vector2 spawnPos = {
            static_cast<float>(GetRandomValue((int)spawner_Area.x, (int)(spawner_Area.x + spawner_Area.width - 32))),
            static_cast<float>(GetRandomValue((int)spawner_Area.y, (int)(spawner_Area.y + spawner_Area.height - 32)))
        };

        Try_Spawn(spawnPos);
        time_Since_Last_Spawn_ = 0.0f;
    }
}

void Enemy_Spawner::Try_Spawn(Vector2 spawn_Position)
{

    if (!CheckCollisionPointRec(spawn_Position, spawner_Area))
        return;

    Rectangle new_Hitbox = { spawn_Position.x, spawn_Position.y, 32.0f, 32.0f };

    if (!Is_Space_Free(new_Hitbox))
        return;

    enemy::Enemy_Base_Class* new_Enemy = createEnemy(spawn_Position);

    if (new_Enemy) {
        enemy_List.push_back(new_Enemy);
    }
}

bool Enemy_Spawner::Is_Space_Free(const Rectangle& newHitbox) const
{
    for (const auto& obstacle : obstacle_List) {
        if (CheckCollisionRecs(newHitbox, obstacle)) {
            return false;
        }
    }

    for (const auto& e : enemy_List) {
        if (CheckCollisionRecs(newHitbox, e->Get_Hitbox())) {}
        {
            return false;
        }
    }
    return true;
}

void Enemy_Spawner::Draw_Spawner_Area() const
{
    DrawRectangleLinesEx(spawner_Area, 2, GREEN);
}
