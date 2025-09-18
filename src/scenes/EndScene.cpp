#include "EndScene.h"
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include "Store.h"
#include "LevelScene.h"
#include "MenuScene.h"
#include "CreditsScene.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config.h.in"

using namespace std::string_literals;

namespace game::scenes
{
    EndScene::EndScene(int final_score)
        : score_(final_score), selected_item_index_(0),
          End_animation_({64, 96}, "assets/graphics/ui/Fire_animation.png", 4, 4, {12, 12, 12, 12}, true)
    {
        SoundManager::GetInstance().PlayMusic("death_music");
        End_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        SetTextureFilter(End_font_.texture, TEXTURE_FILTER_POINT);
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");
        menu_items_ = { "Credits", "Main Menu" };
    }

    EndScene::~EndScene()
    {
        UnloadFont(End_font_);
    }

    void EndScene::Update()
    {
        if (!is_transitioning_)
        {
            time_ += GetFrameTime();

            // --- KONAMI-CODE-LOGIK ---
            if (!konami_code_activated_) {
                int keyPressed = GetKeyPressed();
                if (keyPressed != 0) {
                    key_sequence_.push_back(keyPressed);
                    if (key_sequence_.size() > konami_sequence_.size()) {
                        key_sequence_.erase(key_sequence_.begin());
                    }
                    if (key_sequence_ == konami_sequence_) {
                        konami_code_activated_ = true;
                        SoundManager::GetInstance().PlaySfx("");
                    }
                }
            }

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
                Vector2 text_size = MeasureTextEx(End_font_, menu_items_[i].c_str(), menu_font_size, 2);
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
            // --- Auswahl mit ENTER oder MAUSKLICK ---
            if (IsKeyPressed(KEY_ENTER) || (is_mouse_over_item && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
            {
                // Hier wird der neue Spieler erstellt und die globalen Variablen zurückgesetzt.
                // Dadurch ist ein neuer Run von hier oder vom Menü aus immer sauber.
                game::core::Store::player = std::make_shared<PlayerClass>(Vector2{0,0}, nullptr);
                game::core::Store::next_scene_map = "Tuto_0.json";
                game::core::Store::next_spawn_point = "player_start";

                if (selected_item_index_ == 0) // New Run
                {
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    game::core::Store::stage->ReplaceWithNewScene("end"s, "credits"s, std::make_unique<CreditsScene>());

                }
                else // Main Menu
                {
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    game::core::Store::stage->ReplaceWithNewScene("end"s, "menu"s, std::make_unique<MenuScene>());
                }
            }
        }
    }

    void EndScene::Draw()
    {
        ClearBackground(background_color_);

        // --- Berechnungen für Animationen ---
        float pulse = sin(time_ * 0.8f) * 0.5f + 0.5f;
        float float_offset = sin(time_ * 0.75f) * 10.0f;

        // Animation Größe
        float anim_scale = 8.5f;
        float anim_width = 64.0f * anim_scale;
        float anim_height = 96.0f * anim_scale;

        // Position der Animation
        Vector2 anim_pos = {
            (game::Config::kStageWidth / 2.0f),
            (game::Config::kStageHeight / 2.0f) - 115.0f
        };

        // --- Pulsierender Licht/Flammen-Effekt ---
        Vector2 light_center = anim_pos;
        float base_radius = 180.0f;
        float light_radius = (base_radius * anim_scale / 6.0f) + pulse * 100.0f;
        Color flame_inner = { 0x58, 0x3c, 0x48, 255 };
        Color flame_outer = { 0x44, 0x34, 0x44, 255 };
        DrawCircleGradient(light_center.x, light_center.y, light_radius * 1.5f, ColorAlpha(flame_outer, 0.7f * (pulse * 0.5f + 0.5f)), BLANK);
        DrawCircleGradient(light_center.x, light_center.y, light_radius, ColorAlpha(flame_inner, 0.9f * (pulse * 0.7f + 0.3f)), BLANK);

        // --- Animation zeichnen ---
        End_animation_.Draw_Current_Frame(anim_pos, WHITE, anim_scale);
        End_animation_.Next_Frame();

        // --- "Game Over" Titel ---
        const char* title = "Nice try!";
        float title_font_size = 150;
        Vector2 title_size = MeasureTextEx(End_font_, title, title_font_size, 2);
        Vector2 title_pos = {(game::Config::kStageWidth / 2.0f) - (title_size.x / 2.0f), game::Config::kStageHeight * 0.1f + float_offset};
        DrawTextEx(End_font_, title, title_pos, title_font_size, 2, primary_text_color_);

        // --- Vignette zeichnen ---
        float vignette_radius = Vector2Distance({0, 0}, {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight}) * 0.7f;
        DrawCircleGradient((int)light_center.x, (int)light_center.y, vignette_radius, ColorAlpha(BLACK, 0.0f), ColorAlpha(BLACK, 0.85f));

        // Score ist jetzt höher, direkt unter dem Titel platziert
        std::string score_text = "Final Score: " + std::to_string(score_);
        float score_font_size = 80;
        Vector2 score_size = MeasureTextEx(End_font_, score_text.c_str(), score_font_size, 2);
        Vector2 score_pos = {
            (game::Config::kStageWidth / 2.0f) - (score_size.x / 2.0f),
            anim_pos.y + (anim_height / 2.0f) - 110.0f
        };
        DrawTextEx(End_font_, score_text.c_str(), score_pos, score_font_size, 2, primary_text_color_);

        // --- Menüpunkte ---
        float menu_font_size = 100;
        float initial_y = game::Config::kStageHeight * 0.75f;
        float spacing = 110;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(End_font_, menu_items_[i].c_str(), menu_font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Color current_color = (i == selected_item_index_) ? selected_text_color_ : text_color_;

            if (i == selected_item_index_)
            {
                float selector_offset_x = sin(time_ * 5.0f) * 5.0f;
                float selector_scale = 3.0f;
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 25 + selector_offset_x;
                float selector_y = text_y + (text_size.y / 2.0f) - (selector_height / 2.0f);
                DrawTextureEx(selector_texture_, {selector_x, selector_y}, 0.0f, selector_scale, primary_text_color_);
            }
            DrawTextEx(End_font_, menu_items_[i].c_str(), {text_x, text_y}, menu_font_size, 2, current_color);
        }

        // --- Custom Cursor ---
        float cursor_scale = 1.3f;
        Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");
        DrawTextureEx(cursor_texture, game::core::Store::mouse_Position, 0.0f, cursor_scale, cursor_color_);

        if (is_transitioning_)
        {
            DrawRectangle(0, 0, game::Config::kStageWidth, game::Config::kStageHeight, Fade(BLACK, transition_alpha_));
        }
    }
}