#include "Level1Spawner.h"
#include <vector>
#include <string>
#include "../config.h.in"

// Enemy List
#include "../enemys/InsectMonster.h"
// #include "../enemys/PLACEHOLDER.h" //PLACEHOLDER


// Der Konstruktor ruft einfach den Konstruktor der Basisklasse auf.
Level1_Spawner::Level1_Spawner(Rectangle spawner_Area,
                               const std::vector<Rectangle>& obstacle_List,
                               std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                               float spawn_Rate,
                               int max_Enemies)
    : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List, spawn_Rate, max_Enemies)
{
    // Lade das Sprite für diesen Spawner-Typ aus der Config.
    this->spawner_sprite = LoadTexture(game::Config::kSpawnerLevel1SpritePath);
}

// Der Destruktor, um Speicherlecks zu vermeiden
Level1_Spawner::~Level1_Spawner()
{
    UnloadTexture(this->spawner_sprite);
}

// Die Implementierung der Draw-Funktion
void Level1_Spawner::Draw()
{
    // Zeichne das Spawner-Sprite an der Position der Hitbox.
    DrawTexture(this->spawner_sprite, (int)this->hitbox.x, (int)this->hitbox.y, WHITE);
}


// Hier passiert die Magie: Der Spawner wählt einen zufälligen Gegner aus.
enemy::Enemy_Base_Class* Level1_Spawner::createEnemy(Vector2 position)
{
    // 1. Definiere den "Pool" an möglichen Gegnern für dieses Level.
    // In Zukunft können wir das auch aus der Config laden.
    std::vector<std::string> enemy_pool = {
        "Insect_Monster"
        // "Poison_Frog",
        // "Nyx"
    };

    // 2. Wähle einen zufälligen Gegner aus dem Pool aus.
    int random_index = GetRandomValue(0, enemy_pool.size() - 1);
    std::string enemy_to_spawn = enemy_pool[random_index];

    // 3. Erstelle eine Instanz des ausgewählten Gegners.
    if (enemy_to_spawn == "Insect_Monster")
    {
        return new enemy::Insect_Monster(position);
    }
    /*
    else if (enemy_to_spawn == "Poison_Frog")
    {
        return new enemy::Poison_Frog(position);
    }
    */

    // Fallback, falls etwas schiefgeht.
    return nullptr;
}