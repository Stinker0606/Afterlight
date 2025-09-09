#include "DeathScene.h"
#include <string>
#include <raylib.h>
#include "Store.h"
#include "LevelScene.h"
#include "MenuScene.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config.h.in"

using namespace std::string_literals;

namespace game::scenes
{
    DeathScene::DeathScene(int final_score)
        : score_(final_score), selected_item_index_(0)
    {
        SoundManager::GetInstance().PlayMusic("death_music");
        death_font_ = LoadFontEx("assets/fonts/alagard.ttf", 300, nullptr, 0);
        SetTextureFilter(death_font_.texture, TEXTURE_FILTER_POINT);
        death_image_ = AssetManager::GetInstance().Load("assets/graphics/ui/Spear_Death2.png");
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");
        menu_items_ = { "New Run", "Main Menu" };
    }

    DeathScene::~DeathScene()
    {
        UnloadFont(death_font_);
    }

    void DeathScene::Update()
    {
        // --- Navigation mit Pfeiltasten ---
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            if (selected_item_index_ < menu_items_.size() - 1) selected_item_index_++;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            if (selected_item_index_ > 0) selected_item_index_--;
        }
        // --- Maus-Hover-Effekt ---
        bool is_mouse_over_item = false;
        float menu_font_size = 100;
        float initial_y = game::Config::kStageHeight * 0.75f;
        float spacing = 110;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(death_font_, menu_items_[i].c_str(), menu_font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Rectangle item_rect = { text_x, text_y, text_size.x, text_size.y };

            // Benutze die skalierte Mausposition aus dem Store
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
        // --- Auswahl mit ENTER oder MAUSKLICK ---
        if (IsKeyPressed(KEY_ENTER) || (is_mouse_over_item && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            SoundManager::GetInstance().PlaySfx("ui_select");

            game::core::Store::player = std::make_shared<PlayerClass>(Vector2{0,0}, nullptr);
            game::core::Store::next_scene_map = "Tuto_0.json";
            game::core::Store::next_spawn_point = "player_start";

            if (selected_item_index_ == 0) // New Run
            {
                game::core::Store::stage->ReplaceWithNewScene("death"s, "gameplay"s, std::make_unique<Level1Scene>());
            }
            else // Main Menu
            {
                game::core::Store::stage->ReplaceWithNewScene("death"s, "menu"s, std::make_unique<MenuScene>());
            }
        }
    }

    void DeathScene::Draw()
    {
    ClearBackground(background_color_);

        // --- "Game Over" Titel ---
        const char* title = "Game Over";
        float title_font_size = 150;
        Vector2 title_size = MeasureTextEx(death_font_, title, title_font_size, 2);
        DrawTextEx(death_font_, title, {(game::Config::kStageWidth / 2.0f) - (title_size.x / 2.0f), game::Config::kStageHeight * 0.1f}, title_font_size, 2, primary_text_color_);

        // --- Bild in der Mitte ---
        float image_scale = 8.5f; // Bild vergrößert
        float scaled_width = death_image_.width * image_scale;
        float scaled_height = death_image_.height * image_scale;
        Vector2 image_pos = {
            (game::Config::kStageWidth / 2.0f) - (scaled_width / 2.0f),
            (game::Config::kStageHeight * 0.45f) - (scaled_height / 2.0f)
        };
        DrawTextureEx(death_image_, image_pos, 0.0f, image_scale, primary_text_color_);

        // --- Score ---
        std::string score_text = "Score: " + std::to_string(score_);
        float score_font_size = 80; // Schriftgröße erhöht
        Vector2 score_size = MeasureTextEx(death_font_, score_text.c_str(), score_font_size, 2);
        Vector2 score_pos = {
            (game::Config::kStageWidth / 2.0f) - (score_size.x / 2.0f),
            image_pos.y + scaled_height - 75
        };
        DrawTextEx(death_font_, score_text.c_str(), score_pos, score_font_size, 2, primary_text_color_);

        // --- Menüpunkte ---
        float menu_font_size = 100; // Schriftgröße erhöht
        float initial_y = game::Config::kStageHeight * 0.75f;
        float spacing = 110;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(death_font_, menu_items_[i].c_str(), menu_font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Color current_color = (i == selected_item_index_) ? selected_text_color_ : text_color_;

            if (i == selected_item_index_)
            {
                float selector_scale = 3.0f;
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 25;
                float selector_y = text_y + (text_size.y / 2.0f) - (selector_height / 2.0f);
                DrawTextureEx(selector_texture_, {selector_x, selector_y}, 0.0f, selector_scale, primary_text_color_);
            }
            DrawTextEx(death_font_, menu_items_[i].c_str(), {text_x, text_y}, menu_font_size, 2, current_color);
        }

        // --- Custom Cursor ---
        float cursor_scale = 1.3f;
        Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");
        DrawTextureEx(cursor_texture, game::core::Store::mouse_Position, 0.0f, cursor_scale, cursor_color_);
    }
}