#include "EnemySpawner.h"

#include "raylib.h"

spawner::spawner(Rectangle spawner_Area,
                 const std::vector<Rectangle>& obstacle_List,
                 std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                 float spawn_Rate,
                 int max_Enemies)
    : spawner_Area(spawner_Area),
      obstacle_List(obstacle_List),
      enemy_List(enemy_List),
      spawn_Rate_(spawn_Rate),
      max_Enemies_(max_Enemies),
      time_Since_Last_Spawn_(0.0f)
{}

void spawner::update(float delta_Time) {
    time_Since_Last_Spawn_ += delta_Time;

    // Spawn nur, wenn Spawnrate eingehalten & Max-Gegner noch nicht erreicht
    if (time_Since_Last_Spawn_ >= (1.0f / spawn_Rate_) &&
        (int)enemy_List.size() < max_Enemies_) {

        // Zufällige Spawnposition im Bereich, angepasst auf Hitboxgröße 32x32
        Vector2 spawnPos = {
            static_cast<float>(GetRandomValue((int)spawner_Area.x, (int)(spawner_Area.x + spawner_Area.width - 32))),
            static_cast<float>(GetRandomValue((int)spawner_Area.y, (int)(spawner_Area.y + spawner_Area.height - 32)))
        };

        trySpawn(spawnPos);
        time_Since_Last_Spawn_ = 0.0f;
    }
}

void spawner::trySpawn(Vector2 spawn_Position) {
    // Spawnposition muss im Spawnerbereich sein
    if (!CheckCollisionPointRec(spawn_Position, spawner_Area))
        return;

    // Hitbox für neuen Gegner an Spawnposition
    Rectangle new_Hitbox = { spawn_Position.x, spawn_Position.y, 32.0f, 32.0f };

    // Prüfe ob Platz frei (keine Kollision mit Wänden oder Gegnern)
    if (!isSpaceFree(new_Hitbox))
        return;

    // Gegner erzeugen
    enemy::Enemy_Base_Class* new_Enemy = createEnemy(spawn_Position);

    if (new_Enemy) {
        enemy_List.push_back(new_Enemy);
    }
}

bool spawner::isSpaceFree(const Rectangle& newHitbox) const {
    // Kollision mit Hindernissen prüfen
    for (const auto& obstacle : obstacle_List) {
        if (CheckCollisionRecs(newHitbox, obstacle)) {
            return false;
        }
    }

    // Kollision mit existierenden Gegnern prüfen
    for (const auto& e : enemy_List) {
        if (CheckCollisionRecs(newHitbox, e->getHitbox())) {
            return false;
        }
    }

    return true;
}

void spawner::drawSpawnerArea() const {
    DrawRectangleLinesEx(spawner_Area, 2, GREEN);
}
