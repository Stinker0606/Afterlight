#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "AssetManager.h"

namespace game::scenes
{
    class PauseScene
    {
    public:
        PauseScene();
        ~PauseScene();

        void Update();
        void Draw();

        bool IsResumeSelected() const;
        bool IsMainMenuSelected() const;

    private:
        Font menu_font_;
        Texture2D selector_texture_;
        std::vector<std::string> menu_items_;
        int selected_item_index_;
        bool resume_selected_;
        bool main_menu_selected_;
        float time_ = 0.0f;
    };
}