//
// Created by Kruse on 14/07/2025.
//

//////Goblin Spawner Header//////
#ifndef GOBLINSPAWNER_H
#define GOBLINSPAWNER_H

#include "EnemySpawner.h"
#include "EnemyBaseClass.h"
#include "Collidable.h"

// Spawner für Goblins
/*lass EnemyBaseSpawner : public Enemy_Spawner {
public:
    EnemyBaseSpawner(Rectangle spawner_Area,
                  const std::vector<Rectangle>& obstacle_List,
                  std::vector<enemy::Enemy_Base_Class*>& enemy_List,
                  float spawn_Rate,
                  int max_Enemies)
            : Enemy_Spawner(spawner_Area, obstacle_List, enemy_List, 5, 10) {}

protected:
    std::vector<enemy::Enemy_Base_Class*> enemy_List;
    Rectangle spawner_Area = {200, 200, 500, 300};
    enemy::Enemy_Base_Class* createEnemy(Vector2 position) override;
     void Tick(float delta_time) override;
     void Draw()override;

};
*/




#endif
