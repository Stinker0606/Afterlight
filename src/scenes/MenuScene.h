#pragma once
#include "Scene.h"
#include <vector>
#include <string>

namespace game::scenes
{
    class MenuScene final : public game::core::Scene
    {
    public:
        MenuScene();
        ~MenuScene() override;

        void Update() override;
        void Draw() override;

        void TriggerFadeIn();

    private:
        Texture2D logo_texture_;
        Texture2D selector_texture_;
        Font menu_font_;

        std::vector<std::string> menu_items_;
        int selected_item_index_;

        float time_ = 0.0f;

        bool is_fading_in_ = false;
        float fade_in_alpha_ = 1.0f;

        bool is_transitioning_ = false;
        float transition_alpha_ = 0.0f;
        const float transition_duration_ = 0.8f;

        Color background_color_;
        Color text_color_;
        Color selected_text_color_;
    };
}