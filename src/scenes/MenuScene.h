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
        // Texturen für den Parallax-Hintergrund
        Texture2D bg_layer_back_;
        Texture2D bg_layer_mid_;
        Texture2D bg_layer_front_;

        // NEU: Variable für die Wolken-Animation
        float cloud_offset_x_ = 0.0f;

        // UI-Elemente
        Texture2D logo_texture_;
        Texture2D selector_texture_;
        Font menu_font_;

        std::vector<std::string> menu_items_;
        int selected_item_index_;
        float time_ = 0.0f;

        // Fade- und Transitions-Variablen
        bool is_fading_in_ = false;
        float fade_in_alpha_ = 1.0f;
        bool is_transitioning_ = false;
        float transition_alpha_ = 0.0f;
        const float transition_duration_ = 0.8f;
        const float transition_duration_play = 3.0f;

        // Farben
        Color text_color_;
        Color selected_text_color_;
        Color cursor_color_;
    };
}