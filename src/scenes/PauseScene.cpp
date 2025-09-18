#include "PauseScene.h"
#include "Store.h"
#include "MenuScene.h"
#include "../config.h.in"
#include "SoundManager.h"
#include <string>

using namespace std::string_literals;

namespace game::scenes
{
    PauseScene::PauseScene()
    {
        menu_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        SetTextureFilter(menu_font_.texture, TEXTURE_FILTER_POINT);
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");
        menu_items_ = { "Resume", "Main Menu" };
        selected_item_index_ = 0;
        resume_selected_ = false;
        main_menu_selected_ = false;
    }

    PauseScene::~PauseScene()
    {
        UnloadFont(menu_font_);
    }

    void PauseScene::Update()
    {
        // Setze die Auswahl zurück
        resume_selected_ = false;
        main_menu_selected_ = false;

        time_ += GetFrameTime();

        // --- Maus-Hover ---
        bool is_mouse_over_item = false;
        float font_size = 100;
        int initial_y = 450;
        int spacing = 120;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Rectangle item_rect = { text_x, text_y, text_size.x, text_size.y };

            if (CheckCollisionPointRec(game::core::Store::mouse_Position, item_rect))
            {
                is_mouse_over_item = true;
                if (selected_item_index_ != i)
                {
                    selected_item_index_ = i;
                    SoundManager::GetInstance().PlaySfx("ui_navigate");
                }
            }
        }

        // Tastatur-Navigation
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            selected_item_index_ = (selected_item_index_ + 1) % menu_items_.size();
            SoundManager::GetInstance().PlaySfx("ui_navigate");
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            selected_item_index_ = (selected_item_index_ + menu_items_.size() - 1) % menu_items_.size();
            SoundManager::GetInstance().PlaySfx("ui_navigate");
        }

        // Auswahl (Tastatur oder Maus)
        if (IsKeyPressed(KEY_ENTER) || (is_mouse_over_item && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            SoundManager::GetInstance().PlaySfx("ui_select");
            if (selected_item_index_ == 0) // Resume
            {
                resume_selected_ = true;
            }
            else if (selected_item_index_ == 1) // Main Menu
            {
                main_menu_selected_ = true;
            }
        }
    }

    void PauseScene::Draw()
    {
        // Semi-transparenter Hintergrund
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

        // Titel
        const char* title = "Paused";
        Vector2 title_size = MeasureTextEx(menu_font_, title, 150, 2);
        DrawTextEx(menu_font_, title, {(game::Config::kStageWidth - title_size.x) / 2.0f, 200}, 150, 2, WHITE);

        // Menüpunkte
        float font_size = 100;
        int initial_y = 450;
        int spacing = 120;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Color current_color = (i == selected_item_index_) ? Color{216, 176, 168, 255} : WHITE;

            if (i == selected_item_index_)
            {
                float selector_offset_x = sin(time_ * 5.0f) * 5.0f;
                float selector_scale = 3.0f;
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 25 + selector_offset_x;
                float selector_y = text_y + (text_size.y / 2.0f) - (selector_height / 2.0f);
                DrawTextureEx(selector_texture_, {selector_x, selector_y}, 0.0f, selector_scale, WHITE);
            }
            DrawTextEx(menu_font_, menu_items_[i].c_str(), {text_x, text_y}, font_size, 2, current_color);
        }

        // --- Custom Cursor ---
        float cursor_scale = 1.3f;
        Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");
        DrawTextureEx(cursor_texture, game::core::Store::mouse_Position, 0.0f, cursor_scale, { 88, 60, 72, 255 });
    }

    bool PauseScene::IsResumeSelected() const { return resume_selected_; }
    bool PauseScene::IsMainMenuSelected() const { return main_menu_selected_; }
}