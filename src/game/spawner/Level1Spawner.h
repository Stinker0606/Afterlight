#pragma once

#include "../EnemySpawner.h"
#include <raylib.h>
#include "../../core/Object_Manager.h"

class Level1_Spawner : public Enemy_Spawner
{
public:
    Level1_Spawner(Rectangle spawner_Area,
                   const std::vector<Rectangle>& obstacle_List,
                   std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                   float spawn_Rate,
                   int max_Enemies,
                   Object_Manager& om);

    ~Level1_Spawner() override;

    /**
     * @brief Überschreibt die Tick-Methode von Collidable mit der Spawner-Logik.
     */
    void Tick(float delta_time) override;

    /**
     * @brief Zeichnet das Spawner-Sprite.
     */
    void Draw() override;

    void On_Collision(std::shared_ptr<Collidable> other) override;
private:
    Texture2D spawner_sprite;
    Object_Manager& object_manager_ref;
    float spawn_timer;
    int current_enemy_count;
    int health_;

protected:
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override;
};
