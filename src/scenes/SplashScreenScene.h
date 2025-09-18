#pragma once
#include "Scene.h"

namespace game::scenes
{
    class SplashScreenScene final : public game::core::Scene
    {
    private:
        enum class State { FADE_IN, HOLD, FADE_OUT, FINISHED };
        State current_state_;

        Texture2D logo_texture_;
        float timer_;
        float alpha_;

    public:
        SplashScreenScene();
        ~SplashScreenScene() override;

        void Update() override;
        void Draw() override;
    };
}