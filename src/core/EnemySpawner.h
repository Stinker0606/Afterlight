#ifndef SPAWNER_H
#define SPAWNER_H

#include <vector>
#include "Enemy_Base_Class.h"
#include "raylib.h"


// Basisklasse für Spawner (abstrakt)
class spawner {
public:
    spawner(Rectangle spawner_Area,
            const std::vector<Rectangle>& obstacle_List,
            std::vector<enemy::Enemy_Base_Class*>& enemy_List,
            float spawn_Rate,
            int max_Enemies);


    virtual ~spawner() = default;

    // Wird jede Frame aufgerufen, steuert automatisches Spawnen
    void update(float delta_Time);

    // Versucht einen Gegner an spawn_Position zu spawnen
    void trySpawn(Vector2 spawn_Position);

    // Zeichnet optional den Spawnerbereich
    void drawSpawnerArea() const;

protected:
    Rectangle spawner_Area;                     // Spawnbereich
    const std::vector<Rectangle>& obstacle_List; // Hindernisse (Wände)
    std::vector<enemy::Enemy_Base_Class*>& enemy_List;           // Referenz auf externe Gegnerliste

    float spawn_Rate_;           // Spawnrate (Gegner pro Sekunde)
    int max_Enemies_;            // Maximal erlaubte Gegneranzahl

    float time_Since_Last_Spawn_; // Zeit seit letztem Spawn

    // Prüft, ob der Platz frei ist (keine Kollision)
    bool isSpaceFree(const Rectangle& newHitbox) const;

    // erzeugt konkreten Gegner
    virtual enemy::Enemy_Base_Class* createEnemy(Vector2 position) = 0;
};

//////BEISPIEL GOBLIN SPAWNER//////
/*
//////Goblin Spawner Header//////
#ifndef GOBLINSPAWNER_H
#define GOBLINSPAWNER_H

#include "EnemySpawner.h"

// Spawner für Goblins
class goblinSpawner : public spawner {
public:
    goblinSpawner(Rectangle spawner_Area,
                  const std::vector<Rectangle>& obstacle_List,
                  std::vector<enemy*>& enemy_List,
                  float spawn_Rate,
                  int max_Enemies)
        : spawner(spawner_Area, obstacle_List, enemy_List, spawn_Rate, max_Enemies) {}

protected:
    enemy* createEnemy(Vector2 position) override;
};


//////Goblin Spawner CPP//////
#include "goblinSpawner.h"
#include "goblin.h"

enemy* goblinSpawner::createEnemy(Vector2 position) {
    return new goblin(position);
}

//// in Main ////

// Liste aller Gegner, die vom Spawner erzeugt werden
std::vector<enemy*> enemy_List;

// Beispielhafte Liste von Hindernissen
std::vector<Rectangle> wall_List = {
    {400, 300, 150, 100},
    {700, 500, 100, 180}
};

// Spawnbereich des Spawners (x, y, Breite, Höhe)
Rectangle spawner_Area = {200, 200, 500, 300};


goblinSpawner myGoblinSpawner(spawner_Area, wall_List, enemy_List, 1.0f, 10);

while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    myGoblinSpawner.update(dt);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Spawnerbereich anzeigen (grüner Rahmen)
    myGoblinSpawner.drawSpawnerArea();

    // Alle Gegner aus der gemeinsamen Liste zeichnen
    for (auto* e : enemy_List) {
        e->draw();
    }

    EndDrawing();
}

*/

#endif



