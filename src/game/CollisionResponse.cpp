//
// Created by $Will on 04.07.2025.
//

#include "CollisionResponse.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "EnemyBaseClass.h"
#include "PlayerBaseClass.h"

void CollisionResponse::Resolve_Overlap(Collidable* obj_A, const Collidable* obj_B)
{
    // Hole die Hitboxen der beiden Objekte.
    Rectangle hitbox_A = obj_A->Get_Hitbox();
    Rectangle hitbox_B = obj_B->Get_Hitbox();

    // Berechne die Überlappung auf beiden Achsen.
    float overlap_X = std::min(hitbox_A.x + hitbox_A.width, hitbox_B.x + hitbox_B.width) - std::max(hitbox_A.x, hitbox_B.x);
    float overlap_Y = std::min(hitbox_A.y + hitbox_A.height, hitbox_B.y + hitbox_B.height) - std::max(hitbox_A.y, hitbox_B.y);

    if (overlap_X < overlap_Y)
    {
        if (hitbox_A.x < hitbox_B.x)
        {
            obj_A->Set_Position({ hitbox_A.x - overlap_X, hitbox_A.y });
        }
        else
        {
            obj_A->Set_Position({ hitbox_A.x + overlap_X, hitbox_A.y });
        }
    }
    else
    {
        if (hitbox_A.y < hitbox_B.y)
        {
            obj_A->Set_Position({ hitbox_A.x, hitbox_A.y - overlap_Y });
        }
        else
        {
            obj_A->Set_Position({ hitbox_A.x, hitbox_A.y + overlap_Y });
        }
    }
}

void CollisionResponse::Apply_Damage(Collidable* target, int damage)
{
    if (!target) return;

    if (auto* player = dynamic_cast<Player_Base_Class*>(target))
    {
        player->Take_Damage(damage);
    }
    else if (auto* enemy = dynamic_cast<enemy::Enemy_Base_Class*>(target))
    {
        enemy->Take_Damage(damage);
    }
}

void CollisionResponse::Mark_For_Destruction(Collidable* obj_To_Destroy)
{
    if (!obj_To_Destroy) return;
    obj_To_Destroy->Mark_For_Destruction();
}