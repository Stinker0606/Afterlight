#include "Level1Spawner.h"

#include <iostream>

#include "../enemys/InsectMonster.h"
#include "../../config.h.in"

Level1_Spawner::Level1_Spawner(Rectangle spawner_Area,
                               const std::vector<Rectangle>& obstacle_List,
                               std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                               float spawn_Rate,
                               int max_Enemies,
                               Object_Manager& om)
    : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List, spawn_Rate, max_Enemies),
      object_manager_ref(om),
      spawn_timer(0.0f),
      current_enemy_count(0)
{
    this->hitbox = spawner_Area;
    this->spawner_sprite = LoadTexture(game::Config::kSpawnerLevel1SpritePath);
    if (this->spawner_sprite.id == 0) {
        std::cerr << "FEHLER: Spawner-Sprite konnte nicht geladen werden unter: " << game::Config::kSpawnerLevel1SpritePath << std::endl;
    }
}

Level1_Spawner::~Level1_Spawner()
{
    if (spawner_sprite.id > 0)
    {
        UnloadTexture(this->spawner_sprite);
    }
}

void Level1_Spawner::Tick(float delta_time)
{
    spawn_timer += delta_time;

    if (spawn_timer >= (1.0f / spawn_Rate_) && current_enemy_count < max_Enemies_)
    {
        // Wir spawnen den Gegner nicht mehr an einer zufälligen Position,
        // sondern an einer festen, sicheren Position leicht außerhalb des Spawners.
        Vector2 spawn_pos = {
            this->hitbox.x + (this->hitbox.width / 2.0f) - 16.0f, // Zentriert auf der X-Achse
            this->hitbox.y - 45.0f                                // Leicht über dem Spawner
        };

        enemy::Enemy_Base_Class* new_enemy_raw = createEnemy(spawn_pos);

        if (new_enemy_raw)
        {
            object_manager_ref.AddObject(std::shared_ptr<enemy::Enemy_Base_Class>(new_enemy_raw));
            current_enemy_count++;
            spawn_timer = 0.0f;
        }
    }
}

void Level1_Spawner::Draw()
{
    // Zeichne das Spawner-Sprite mit der korrekten Transparenz für den Nebel.
    DrawTextureV(this->spawner_sprite, {(float)this->hitbox.x, (float)this->hitbox.y}, Fade(WHITE, this->visibility_alpha));
}

enemy::Enemy_Base_Class* Level1_Spawner::createEnemy(Vector2 position)
{
    return new enemy::Insect_Monster(position);
}
