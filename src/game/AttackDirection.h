//
// Created by Manza on 6/22/2025.
//

#pragma once

/**
 * @brief Defines the 8 discrete directions for attacks, plus a null-state.
 */
enum class Attack_Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    NONE
};