#pragma once
#include <Scene.h>
#include "Screen.h"
#include "../game/PlayerClassOne.h"
#include "DeltaTimeMachine.h"
#include "Cam.h"

namespace game::scenes
{
    class GameScene final : public game::core::Scene
    {
    private:
        Vector2 sp{730,130};
        Rectangle wb{0,0,game::Config::kStageWidth,game::Config::kStageHeight};
        Collision_Manager* p_cm = new Collision_Manager(wb,objectManager.managed_objects);
        Player_Class_One mp{sp};
        DT::timemachine dtm;

        float key_cooldown;
        const float KEY_PRESS_DELAY = 0.1f;

        void Handle_Fog_Controls(float delta_time);
        void Draw_Fog_UI();

    public:
        GameScene();
        ~GameScene() override;
        void Update() override;
        void Draw() override;

        int Level_Nbr = 1;
        Screen screen {&Level_Nbr};
        std::shared_ptr<Cam> cam;
        Object_Manager objectManager;
    };
}