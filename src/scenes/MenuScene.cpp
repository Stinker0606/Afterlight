#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include <cmath>
#include "Store.h"
#include "LevelScene.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config.h.in"
#include "SettingsScene.h"
#include "CreditsScene.h"


using namespace std::string_literals;

namespace game::scenes
{
    MenuScene::MenuScene()
    {
        HideCursor();
        SoundManager::GetInstance().PlayMusic("menu_music");

        // Lade die Hintergrund-Layer
        bg_layer_back_ = AssetManager::GetInstance().Load("assets/graphics/ui/menu_bg_back.png");
        bg_layer_mid_ = AssetManager::GetInstance().Load("assets/graphics/ui/menu_bg_mid.png");
        bg_layer_front_ = AssetManager::GetInstance().Load("assets/graphics/ui/menu_bg_front.png");

        // Lade die UI-Assets
        logo_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/Afterlight_logo_glow.png");
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");
        menu_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        SetTextureFilter(menu_font_.texture, TEXTURE_FILTER_POINT);

        menu_items_ = { "Play", "Settings", "Credits", "Quit" };
        selected_item_index_ = 0;

        // Farben
        text_color_ = { 88, 60, 72, 255 };
        selected_text_color_ = { 216, 176, 168, 255 };
        cursor_color_ = { 88, 60, 72, 255 };
    }

    MenuScene::~MenuScene()
    {
        UnloadFont(menu_font_);
    }

    void MenuScene::TriggerFadeIn()
    {
        is_fading_in_ = true;
        fade_in_alpha_ = 1.0f;
    }

    void MenuScene::Update()
    {
        // Fade-In-Logik...
        if (is_fading_in_)
        {
            fade_in_alpha_ -= GetFrameTime() / 1.5f;
            if (fade_in_alpha_ <= 0.0f)
            {
                fade_in_alpha_ = 0.0f;
                is_fading_in_ = false;
            }
            return;
        }

        if (is_transitioning_)
        {
            transition_alpha_ += GetFrameTime() / transition_duration_play;
            if (transition_alpha_ >= 1.0f)
            {
                game::core::Store::stage->ReplaceWithNewScene("menu"s, "gameplay"s, std::make_unique<Level1Scene>());
            }
            return;
        }

        time_ += GetFrameTime();

        // Bewege die Wolken langsam nach links
        cloud_offset_x_ -= 5.0f * GetFrameTime(); // 5 Pixel pro Sekunde
        // Wenn die Wolken komplett durchgelaufen sind, setze sie zurück für einen nahtlosen Loop
        if (cloud_offset_x_ <= -game::Config::kStageWidth) {
            cloud_offset_x_ = 0;
        }

        // Navigation mit Pfeiltasten
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            SoundManager::GetInstance().PlaySfx("ui_navigate");
            if (selected_item_index_ < menu_items_.size() - 1) selected_item_index_++;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            SoundManager::GetInstance().PlaySfx("ui_navigate");
            if (selected_item_index_ > 0) selected_item_index_--;
        }

        // --- Maus-Logik mit game::core::Store::mouse_Position ---
        bool is_mouse_over_item = false;
        float font_size = 120;
        int initial_y = 420;
        int spacing = 140;

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

        // Auswahl mit ENTER oder Mausklick
        if (IsKeyPressed(KEY_ENTER) || (is_mouse_over_item && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            switch (selected_item_index_)
            {
                case 0: // Play
                    SoundManager::GetInstance().StopCurrentMusic();
                    SoundManager::GetInstance().PlaySfx("game_start");
                    is_transitioning_ = true;
                    break;
                case 1: // Settings
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    game::core::Store::stage->ReplaceWithNewScene("menu", "settings", std::make_unique<SettingsScene>());
                break;
                case 2: // Credits
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    game::core::Store::stage->ReplaceWithNewScene("menu", "credits", std::make_unique<CreditsScene>());
                    break;
                case 3: // Quit
                    CloseWindow();
                    exit(0);
                    break;
            }
        }
    }

    void MenuScene::Draw()
    {
        ClearBackground(Color{ 0, 44, 56, 255 });

        // --- PARALLAX-EFFEKT & WOLKEN-ANIMATION ---
        Vector2 screen_center = { game::Config::kStageWidth / 2.0f, game::Config::kStageHeight / 2.0f };
        Vector2 mouse_pos = game::core::Store::mouse_Position;
        float max_offset = 25.0f; // Etwas stärkerer Effekt

        float offset_x = (mouse_pos.x - screen_center.x) / screen_center.x * max_offset;
        float offset_y = (mouse_pos.y - screen_center.y) / screen_center.y * max_offset;

        // Skaliere alle Ebenen korrekt und wende den Parallax-Effekt an
        float scale_mid = (float)game::Config::kStageWidth / bg_layer_mid_.width;
        float scale_front = (float)game::Config::kStageWidth / bg_layer_front_.width;


        // --- Wolken zeichnen ---
        DrawTextureEx(bg_layer_back_, { cloud_offset_x_ + (offset_x * 0.2f), offset_y * 0.2f }, 0.0f, (float)game::Config::kStageWidth / bg_layer_back_.width, WHITE);
        DrawTextureEx(bg_layer_back_, { game::Config::kStageWidth + cloud_offset_x_ + (offset_x * 0.2f), offset_y * 0.2f }, 0.0f, (float)game::Config::kStageWidth / bg_layer_back_.width, WHITE);

        // Berge und Schwerter
        float zoom_factor = 1.05f; // 5% größer
        DrawTextureEx(bg_layer_mid_, { (offset_x * 0.6f) - (game::Config::kStageWidth * (zoom_factor - 1) / 2), (offset_y * 0.6f) - (game::Config::kStageHeight * (zoom_factor - 1) / 2) }, 0.0f, scale_mid * zoom_factor, WHITE);
        DrawTextureEx(bg_layer_front_, { (offset_x * 0.6f) - (game::Config::kStageWidth * (zoom_factor - 1) / 2), (offset_y * 0.6f) - (game::Config::kStageHeight * (zoom_factor - 1) / 2) }, 0.0f, scale_front * zoom_factor, WHITE);

        // --- Logo ---
        float scale = 0.7f;
        float logo_width = logo_texture_.width * scale;
        float logo_height = logo_texture_.height * scale;
        float logo_x = (game::Config::kStageWidth / 2.0f) - (logo_width / 2.0f);
        float logo_y = -180;
        DrawTexturePro(logo_texture_, { 0, 0, (float)logo_texture_.width, (float)logo_texture_.height }, { logo_x, logo_y, logo_width, logo_height }, {0,0}, 0.0f, WHITE);


        // --- Menüpunkte ---
        float font_size = 120;
        int initial_y = 420;
        int spacing = 140;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (game::Config::kStageWidth / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Color current_color = (i == selected_item_index_) ? selected_text_color_ : text_color_;

            if (i == selected_item_index_)
            {
                float selector_offset_x = sin(time_ * 2.5f) * 5.0f;
                float selector_scale = 3.5f;
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 30 + selector_offset_x;
                float selector_y = text_y + (text_size.y / 2.0f) - (selector_height / 2.0f);
                DrawTextureEx(selector_texture_, {selector_x, selector_y}, 0.0f, selector_scale, WHITE);
            }
            DrawTextEx(menu_font_, menu_items_[i].c_str(), {text_x, text_y}, font_size, 2, current_color);
        }

        // --- Custom Cursor ---
        float cursor_scale = 1.3f;
        Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");
        DrawTextureEx(cursor_texture, game::core::Store::mouse_Position, 0.0f, cursor_scale, cursor_color_);

        // Fade-Effekte
        if (is_transitioning_)
        {
            DrawRectangle(0, 0, game::Config::kStageWidth, game::Config::kStageHeight, Fade(BLACK, transition_alpha_));
        }
        if (is_fading_in_)
        {
            DrawRectangle(0, 0, game::Config::kStageWidth, game::Config::kStageHeight, Fade(BLACK, fade_in_alpha_));
        }
    }
}