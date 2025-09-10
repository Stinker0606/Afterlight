#pragma once
#include "Scene.h"
#include <string>
#include <vector>

namespace game::scenes
{
    struct CreditLine
    {
        std::string text_;
        float font_size_;
    };

    class CreditsScene final : public game::core::Scene
    {
    public:
        CreditsScene();
        ~CreditsScene() override;

        void Update() override;
        void Draw() override;

    private:
        Font menuFont_;
        Texture2D background_credits_;
        float time_ = 0.0f;
        float scrollY_ = 0.0f;
        float totalHeight_ = 0.0f; //Höhe aller Zeilen für den Loop

        std::vector<CreditLine> creditsLines_;

        float default_font_size_ = 60.0f;
        float big_font_size_ = 120.0f;


    };
}
