#include "SettingsScene.h"
#include "SettingsManager.h"
#include "MenuScene.h"
#include "Store.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "raymath.h"
#include "../config.h.in"

namespace game::scenes
{
    SettingsScene::SettingsScene()
    {
        menu_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        SetTextureFilter(menu_font_.texture, TEXTURE_FILTER_POINT);
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");
        display_mode_names_ = { "Windowed", "Borderless", "Fullscreen" };

        //Hintergrund laden
        background_settings_ =AssetManager::GetInstance().Load("assets/graphics/backgrounds/UI/Background_Setting.png");

    }

    SettingsScene::~SettingsScene()
    {
        UnloadFont(menu_font_);
    }

    void SettingsScene::Update()
    {
        time_ += GetFrameTime();
        auto& settings = SettingsManager::GetInstance();
        Vector2 mouse_pos = game::core::Store::mouse_Position;

        int menu_item_count = 7;

        // --- Helfer-Funktion für Lautstärke-Änderung in 5er-Schritten ---
        auto change_volume = [](float current_volume, float change) {
            int current_percent = static_cast<int>(roundf(current_volume * 100.0f));
            int new_percent = current_percent + static_cast<int>(change * 100.0f);
            new_percent = (new_percent / 5) * 5; // Raste auf den nächsten 5er-Schritt ein
            return Clamp((float)new_percent / 100.0f, 0.0f, 1.0f);
        };

        // --- Interaktionsbereiche ---
        Rectangle display_mode_rect = {1200, 140, 550, 80};
        Rectangle resolution_rect = {1200, 240, 550, 80};
        Rectangle vsync_rect = {1200, 340, 550, 80};
        Rectangle minus_master_rect = {1180, 440, 65, 80};
        Rectangle plus_master_rect = {1320, 440, 125, 80};
        Rectangle minus_music_rect = {1180, 540, 65, 80};
        Rectangle plus_music_rect = {1320, 540, 125, 80};
        Rectangle minus_sfx_rect = {1180, 640, 65, 80};
        Rectangle plus_sfx_rect = {1320, 640, 125, 80};
        Vector2 back_size = MeasureTextEx(menu_font_, "Back", 80, 2);
        Rectangle back_button_rect = {(game::Config::kStageWidth - back_size.x) / 2.0f, 760, back_size.x, back_size.y};

        // --- Maus-Hover-Logik ---
        if (CheckCollisionPointRec(mouse_pos, { 350, 140, 1200, 80 })) selected_item_index_ = 0;
        else if (CheckCollisionPointRec(mouse_pos, { 350, 240, 1200, 80 })) selected_item_index_ = 1;
        else if (CheckCollisionPointRec(mouse_pos, { 350, 340, 1200, 80 })) selected_item_index_ = 2;
        else if (CheckCollisionPointRec(mouse_pos, { 350, 440, 1200, 80 })) selected_item_index_ = 3;
        else if (CheckCollisionPointRec(mouse_pos, { 350, 540, 1200, 80 })) selected_item_index_ = 4;
        else if (CheckCollisionPointRec(mouse_pos, { 350, 640, 1200, 80 })) selected_item_index_ = 5;
        else if (CheckCollisionPointRec(mouse_pos, back_button_rect)) selected_item_index_ = 6;

        // --- Maus-Klick-Logik ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse_pos, display_mode_rect)) {
                int mode = (int)settings.GetDisplayMode();
                settings.SetDisplayMode((DisplayMode)((mode + 1) % 3));
                settings.ApplyDisplaySettings();
                SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if (CheckCollisionPointRec(mouse_pos, resolution_rect)) {
                 int index = settings.GetCurrentResolutionIndex();
                 int max_index = settings.GetAvailableResolutions().size() - 1;
                 settings.SetCurrentResolutionIndex((index + 1) % (max_index + 1));
                 settings.ApplyDisplaySettings();
                 SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if (CheckCollisionPointRec(mouse_pos, vsync_rect)) {
                 settings.SetVsync(!settings.IsVsyncEnabled());
                 settings.ApplyDisplaySettings();
                 SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if (CheckCollisionPointRec(mouse_pos, minus_master_rect) || CheckCollisionPointRec(mouse_pos, plus_master_rect)) {
                float change = CheckCollisionPointRec(mouse_pos, minus_master_rect) ? -0.05f : 0.05f;
                settings.SetMasterVolume(change_volume(settings.GetMasterVolume(), change));
                SoundManager::GetInstance().UpdateMusicVolume();
                SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if (CheckCollisionPointRec(mouse_pos, minus_music_rect) || CheckCollisionPointRec(mouse_pos, plus_music_rect)) {
                float change = CheckCollisionPointRec(mouse_pos, minus_music_rect) ? -0.05f : 0.05f;
                settings.SetMusicVolume(change_volume(settings.GetMusicVolume(), change));
                SoundManager::GetInstance().UpdateMusicVolume();
                SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if (CheckCollisionPointRec(mouse_pos, minus_sfx_rect) || CheckCollisionPointRec(mouse_pos, plus_sfx_rect)) {
                float change = CheckCollisionPointRec(mouse_pos, minus_sfx_rect) ? -0.05f : 0.05f;
                settings.SetSfxVolume(change_volume(settings.GetSfxVolume(), change));
                SoundManager::GetInstance().PlaySfx("ui_select");
            }
            else if(CheckCollisionPointRec(mouse_pos, back_button_rect)) {
                settings.SaveSettings();
                SoundManager::GetInstance().PlaySfx("ui_select");
                game::core::Store::stage->ReplaceWithNewScene("settings", "menu", std::make_unique<MenuScene>());
            }
        }

        // --- Tastatur-Navigation & Bedienung ---
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) selected_item_index_ = (selected_item_index_ + 1) % menu_item_count;
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) selected_item_index_ = (selected_item_index_ + menu_item_count - 1) % menu_item_count;

        bool pressed_left = IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
        bool pressed_right = IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);

        if (pressed_left || pressed_right)
        {
            switch(selected_item_index_) {
                case 0: // Display Mode
                    {
                    int mode = (int)settings.GetDisplayMode();
                    int change = pressed_left ? 2 : 1;
                    settings.SetDisplayMode((DisplayMode)((mode + change) % 3));
                    settings.ApplyDisplaySettings();
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
                    }
                case 1: // Resolution
                    {
                    int index = settings.GetCurrentResolutionIndex();
                    int max_index = settings.GetAvailableResolutions().size() - 1;
                    int change = pressed_left ? -1 : 1;
                    settings.SetCurrentResolutionIndex((index + change + max_index + 1) % (max_index + 1));
                    settings.ApplyDisplaySettings();
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
                    }
                case 2: // V-Sync
                    settings.SetVsync(!settings.IsVsyncEnabled());
                    settings.ApplyDisplaySettings();
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
                case 3: // Master Volume
                    settings.SetMasterVolume(change_volume(settings.GetMasterVolume(), pressed_left ? -0.05f : 0.05f));
                    SoundManager::GetInstance().UpdateMusicVolume();
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
                case 4: // Music Volume
                    settings.SetMusicVolume(change_volume(settings.GetMusicVolume(), pressed_left ? -0.05f : 0.05f));
                    SoundManager::GetInstance().UpdateMusicVolume();
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
                case 5: // SFX Volume
                    settings.SetSfxVolume(change_volume(settings.GetSfxVolume(), pressed_left ? -0.05f : 0.05f));
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    break;
            }
        }

        if (selected_item_index_ == 6 && IsKeyPressed(KEY_ENTER)) {
             settings.SaveSettings();
             SoundManager::GetInstance().PlaySfx("ui_select");
             game::core::Store::stage->ReplaceWithNewScene("settings", "menu", std::make_unique<MenuScene>());
        }
    }

    void SettingsScene::Draw()
    {
        //Hintergrund zeichnen
        DrawTextureEx(background_settings_, {0, 0}, 0.0f, 11.55f, WHITE);
        auto& settings = SettingsManager::GetInstance();
        float selector_offset_x = sin(time_ * 2.5f) * 5.0f;

        // --- Helfer zum Zeichnen ---
        auto draw_setting_text = [&](int index, const std::string& name, Vector2 pos) {
            Color color = (selected_item_index_ == index) ? Color{216, 176, 168, 255} : WHITE;
            DrawTextEx(menu_font_, name.c_str(), pos, 80, 2, color);
            if (selected_item_index_ == index) {
                DrawTextureEx(selector_texture_, {pos.x - 120 + selector_offset_x, pos.y - 15}, 0.0f, 3.0f, WHITE);
            }
        };

        // --- Alle UI-Elemente zeichnen ---
        draw_setting_text(0, "Display Mode", {350, 140});
        DrawTextEx(menu_font_, ("< " + display_mode_names_[(int)settings.GetDisplayMode()] + " >").c_str(), {1200, 140}, 80, 2, WHITE);

        Vector2 res = settings.GetCurrentResolution();
        std::string res_text = std::to_string((int)res.x) + "x" + std::to_string((int)res.y);
        draw_setting_text(1, "Resolution", {350, 240});
        DrawTextEx(menu_font_, ("< " + res_text + " >").c_str(), {1200, 240}, 80, 2, WHITE);

        draw_setting_text(2, "V-Sync", {350, 340});
        DrawTextEx(menu_font_, (settings.IsVsyncEnabled() ? "< On >" : "< Off >"), {1200, 340}, 80, 2, WHITE);

        draw_setting_text(3, "Master Volume", {350, 440});
        DrawTextEx(menu_font_, ("- " + std::to_string((int)(settings.GetMasterVolume() * 100)) + " +").c_str(), {1200, 440}, 80, 2, WHITE);

        draw_setting_text(4, "Music Volume", {350, 540});
        DrawTextEx(menu_font_, ("- " + std::to_string((int)(settings.GetMusicVolume() * 100)) + " +").c_str(), {1200, 540}, 80, 2, WHITE);

        draw_setting_text(5, "SFX Volume", {350, 640});
        DrawTextEx(menu_font_, ("- " + std::to_string((int)(settings.GetSfxVolume() * 100)) + " +").c_str(), {1200, 640}, 80, 2, WHITE);

        Vector2 back_size = MeasureTextEx(menu_font_, "Back", 80, 2);
        draw_setting_text(6, "Back", {(game::Config::kStageWidth - back_size.x) / 2.0f, 760});

        // --- Custom Cursor ---
        float cursor_scale = 1.3f;
        Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");
        DrawTextureEx(cursor_texture, game::core::Store::mouse_Position, 0.0f, cursor_scale, { 88, 60, 72, 255 });
    }
}