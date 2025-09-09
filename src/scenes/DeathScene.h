#pragma once

#include "Scene.h"
#include <vector>
#include <string>

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
        Texture2D death_image_;
        Texture2D selector_texture_;

        std::vector<std::string> menu_items_;
        int selected_item_index_;

        // Farben für die UI
        Color background_color_ = { 0, 44, 56, 255 };
        Color text_color_ = { 216, 176, 168, 150 };
        Color selected_text_color_ = { 216, 176, 168, 255 };
        Color primary_text_color_ = { 252, 252, 252, 255 };
        Color cursor_color_ = { 88, 60, 72, 255 };
    };
}