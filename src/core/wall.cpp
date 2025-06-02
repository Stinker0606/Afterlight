//
// Created by $Will on 01.06.2025.
//
#include "wall_collisions.h"
#include "collision_check.h"
#include "DeltaTimeMachine.h"
#include "Playerinput.h"

namespace game::core
{
    void HandleWallCollisions(Rectangle& playerRect, float& playerX,
        float& playerY, float playerSpeed, DT::timemachine& deltaTimeMachine, Player::Direction direction)
    {
        float dt = deltaTimeMachine.Get_Dt();

        for (auto& wall : walls) //Variable Wall soll aus dem Vector kommen in welchem die Walls gespeichert sind
            {
            if (wall->CheckCollision(playerRect))
                {
                if (auto* movable = dynamic_cast<Movable_Wall*>(wall))
                {
                    movable ->TriggerMove(direction);
                }

                // Blockiere Bewegung: Position zurücksetzen
                playerRect.x = playerX;
                playerRect.y = playerY;
            }

            wall->Update(dt);
        }
    }

} // namespace game::core