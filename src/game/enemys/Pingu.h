#pragma once

#include "../EnemyBaseClass.h"

class Object_Manager;

namespace enemy
{
    class Pingu : public Enemy_Base_Class
    {
    private:
        Vector2 last_player_position_;

    public:
        Pingu(Vector2 start_position, Object_Manager& om, bool use_fog = true);
        ~Pingu() override = default;

        void Update_AI(float delta_time, Vector2 player_position) override;
        void Melee_Attack() override;
        void Range_Attack() override {}; // Hat keinen Fernkampf
        void Draw() override;

        void PlayHitSound() override {};
        void PlayDeathSound() override {};
    };
}