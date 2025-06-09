//
// Created by $Will on 09.06.2025.
//

#ifndef ENEMYCLASS1_H
#define ENEMYCLASS1_H

#include "EnemyBaseClass.h"

namespace enemy
{
    class Enemy_Class_1 : public Enemy_Base_Class
    {
    private:
        Texture2D sprite;

    public:
        Enemy_Class_1(Vector2 start_position);
        ~Enemy_Class_1();

        // Implementierung der abstrakten Methoden
        void Attack() override;
        void Draw() override;
        void Pathfinding() override;
    };
}
#endif //ENEMYCLASS1_H
