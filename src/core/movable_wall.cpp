//
// Created by $Will on 01.06.2025.
//
#include "wall_collisions.h"
#include "collision_check.h"
#include "../config.h.in"

//Diese komplette Datei ist Teamspezifisch
namespace game::core
{

    void MovableWall::TriggerMove(Player::Direction)
    {
        if (!isMoving)
        {
            isMoving = true;
            movedDistance = 0.0f;

            switch (direction)
            {
                case Player::Up:    direction = {0, -1}; break;
                case Player::Down:  direction = {0, 1}; break;
                case Player::Left:  direction = {-1, 0}; break;
                case Player::Right: direction = {1, 0}; break;
                default:            isMoving = false; break;
            }
        }
    }

    void MovableWall::Update(float dt)
    {
        if (isMoving && movedDistance < Config::tile_Size) {
            float moveStep = speed * dt;

            if (movedDistance + moveStep > Config::tile_Size) {
                moveStep = 32.0f - movedDistance;
                isMoving = false;
            }

            rect.x += direction.x * moveStep;
            rect.y += direction.y * moveStep;
            movedDistance += moveStep;
        }
    }

} // namespace game::core