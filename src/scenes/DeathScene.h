#pragma once

#include "Scene.h"
#include <vector>
#include <string>
#include <raymath.h>
#include "../core/ControllableAnimations.h"

namespace game::scenes
{
    class DeathScene final : public game::core::Scene
    {
    public:
        // Der Konstruktor nimmt den finalen Score aus dem Level entgegen
        explicit DeathScene(int final_score);
        ~DeathScene() override;

        void Update() override;
        void Draw() override;

    private:
        int score_;
        Font death_font_;
        Texture2D selector_texture_;
        ControllableAnimations death_animation_;

        std::vector<std::string> menu_items_;
        int selected_item_index_;

        float time_ = 0.0f; // Hinzugefügt, um die Zeit für Animationen zu verfolgen

        bool is_transitioning_ = false;
        float transition_alpha_ = 0.0f;
        const float transition_duration_ = 0.8f;

        // Farben für die UI
        Color background_color_ = { 10, 20, 25, 255 }; // Etwas dunkler für mehr Kontrast
        Color text_color_ = { 216, 176, 168, 150 };
        Color selected_text_color_ = { 216, 176, 168, 255 };
        Color primary_text_color_ = { 252, 252, 252, 255 };
        Color cursor_color_ = { 88, 60, 72, 255 };

        // Neue Farben für den Flammeneffekt, passend zur Farbpalette
        Color flame_outer_color_ = { 200, 80, 40, 255 };
        Color flame_inner_color_ = { 255, 180, 50, 255 };
    };
}