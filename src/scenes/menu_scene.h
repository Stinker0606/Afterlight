#pragma once

#include <scene.h>
#include "screen.h"

namespace game::scenes {
    class MenuScene final : public game::core::Scene {
    public:
        MenuScene();

        ~MenuScene() override ;

        void Update() override;

        void Draw() override;

        Screen screen;
    };
}