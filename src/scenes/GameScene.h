#pragma once
#include <Scene.h>

namespace game::scenes
{
    class GameScene final : public game::core::Scene
    {
    public:
        GameScene();

        ~GameScene() override;

        void Update() override;

        void Draw() override;

    private:
        // Add private members here...
    };
}