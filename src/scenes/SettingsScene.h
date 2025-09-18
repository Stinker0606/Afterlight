#pragma once
#include "Scene.h"
#include <vector>
#include <string>

namespace game::scenes
{
    class SettingsScene final : public game::core::Scene
    {
    public:
        SettingsScene();
        ~SettingsScene() override;

        void Update() override;
        void Draw() override;

    private:
        Font menu_font_;
        Texture2D selector_texture_;
        Texture2D background_settings_;

        Texture2D plus_texture_;
        Texture2D minus_texture_;

        float time_ = 0.0f;

        int selected_item_index_ = 0;
        std::vector<std::string> display_mode_names_;
    };
}