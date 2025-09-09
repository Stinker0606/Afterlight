#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include <cmath>
#include "Store.h"
#include "LevelScene.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config_audio.h.in"

using namespace std::string_literals;

namespace game::scenes
{
    MenuScene::MenuScene()
    {
        // Startet den Cursor
        HideCursor();

        // Starte die Hauptmenü-Musik
        SoundManager::GetInstance().PlayMusic("menu_music");

        // Lade die Assets über den AssetManager
        logo_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/Afterlight_logo.png");
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");

        // Lade die Schriftart, die bereits im Projekt ist
        menu_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        // Setze den Texturfilter für die Schriftart auf FILTER_POINT für scharfe Pixel
        SetTextureFilter(menu_font_.texture, TEXTURE_FILTER_POINT);

        // Definiere die Menüpunkte
        menu_items_ = { "Play", "Settings", "Credits", "Quit" };
        selected_item_index_ = 0; // "Play" ist zu Beginn ausgewählt

        // Definiere die Farben (basierend auf deinem Feedback)
        background_color_ = { 0, 44, 56, 255 };
        text_color_ = { 68, 52, 68, 255 };
        selected_text_color_ = { 216, 176, 168, 255 };
    }

    MenuScene::~MenuScene()
    {
        // Assets werden vom AssetManager verwaltet und müssen hier nicht entladen werden.
        UnloadFont(menu_font_);
    }

    void MenuScene::TriggerFadeIn()
    {
        is_fading_in_ = true;
        fade_in_alpha_ = 1.0f;
    }

    void MenuScene::Update()
    {
        if (is_fading_in_)
        {
            fade_in_alpha_ -= GetFrameTime() / 3.2f; // Fade-In über 1 Sekunde
            if (fade_in_alpha_ <= 0.0f)
            {
                fade_in_alpha_ = 0.0f;
                is_fading_in_ = false;
            }
            return; // Stoppe weitere Updates während des Fadens
        }

        if (!is_transitioning_)
        {
        // Zeit für die Animation aktualisieren
        time_ += GetFrameTime();

        // Navigation mit Pfeiltasten - angepasst, um nicht im Kreis zu springen
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            SoundManager::GetInstance().PlaySfx("ui_navigate");
            if (selected_item_index_ < menu_items_.size() - 1)
            {
                selected_item_index_++;
            }
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            SoundManager::GetInstance().PlaySfx("ui_navigate");
            if (selected_item_index_ > 0)
            {
                selected_item_index_--;
            }
        }

        // Maus-Navigation und Klick-Logik
        bool is_mouse_over_item = false;
        for (int i = 0; i < menu_items_.size(); ++i)
        {
            float font_size = 120;
            int initial_y = 420;
            int spacing = 140;
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (GetScreenWidth() / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);
            Rectangle item_rect = { text_x, text_y, text_size.x, text_size.y };

            // Prüfe, ob die Maus über einem Menüpunkt ist
            if (CheckCollisionPointRec(GetMousePosition(), item_rect))
            {
                is_mouse_over_item = true;
                if (selected_item_index_ != i)
                {
                    selected_item_index_ = i;
                    SoundManager::GetInstance().PlaySfx("ui_navigate");
                }
            }
        }


        // Auswahl mit ENTER
        if (IsKeyPressed(KEY_ENTER) || (is_mouse_over_item && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            switch (selected_item_index_)
            {
                case 0: // Play
                    // Wechsle zur Haupt-Spielszene (Level1Scene)
                    is_transitioning_ = true;
                    SoundManager::GetInstance().PlaySfx("game_start");
                    break;
                case 1: // Settings
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    // Noch keine Funktion
                    break;
                case 2: // Credits
                    SoundManager::GetInstance().PlaySfx("ui_select");
                    // Noch keine Funktion
                    break;
                case 3: // Quit
                    // Schließt die Anwendung. CloseWindow() setzt das Flag, das die Hauptschleife in Game.cpp beendet.
                    CloseWindow();
                    exit(0);
                    break;
            }
        }
    }
        else // Wenn wir in einer Transition sind:
        {
            transition_alpha_ += GetFrameTime() / transition_duration_;
            if (transition_alpha_ >= 1.0f)
            {
                // Wenn der Bildschirm schwarz ist, wechsle die Szene
                game::core::Store::stage->ReplaceWithNewScene("menu"s, "gameplay"s, std::make_unique<Level1Scene>());
            }
        }
    }

 void MenuScene::Draw()
    {
        ClearBackground(background_color_);

        // --- Logo zeichnen ---
        float scale = 0.7f; // Skalierungsfaktor für das Logo
        float logo_width = logo_texture_.width * scale;
        float logo_height = logo_texture_.height * scale;
        float logo_x = (GetScreenWidth() / 2.0f) - (logo_width / 2.0f);
        float logo_y = -180;

        DrawTexturePro(
            logo_texture_,
            { 0, 0, (float)logo_texture_.width, (float)logo_texture_.height },
            { logo_x, logo_y, logo_width, logo_height },
            { 0, 0 },
            0.0f,
            WHITE
        );

        // --- Menüpunkte zeichnen ---
        float font_size = 120;
        int initial_y = 420;
        int spacing = 140;

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (GetScreenWidth() / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);

            Color current_color = (i == selected_item_index_) ? selected_text_color_ : text_color_;

            // Auswahl-Pfeil zeichnen
            if (i == selected_item_index_)
            {
                float selector_offset_x = sin(time_ * 2.5f) * 5.0f; // Schwingt 5 Pixel nach links und rechts
                float selector_scale = 3.5f; // Skalierungsfaktor für den Pfeil
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 30 + selector_offset_x;
                float selector_y = text_y + (text_size.y / 2.0f) - (selector_height / 2.0f);

                DrawTexturePro(
                    selector_texture_,
                    { 0, 0, (float)selector_texture_.width, (float)selector_texture_.height },
                    { selector_x, selector_y, selector_width, selector_height },
                    { 0, 0 },
                    0.0f,
                    WHITE
                );
            }

            DrawTextEx(menu_font_, menu_items_[i].c_str(), {text_x, text_y}, font_size, 2, current_color);

            float cursor_scale = 1.3f;
            Texture2D cursor_texture = AssetManager::GetInstance().Load("assets/graphics/ui/cursor.png");

            // Berechne die neue Größe basierend auf dem Skalierungsfaktor
            float cursor_width = cursor_texture.width * cursor_scale;
            float cursor_height = cursor_texture.height * cursor_scale;

            // Zeichne die Textur mit der neuen Größe
            DrawTexturePro(
                cursor_texture,
                { 0, 0, (float)cursor_texture.width, (float)cursor_texture.height },
                { GetMousePosition().x, GetMousePosition().y, cursor_width, cursor_height },
                { 0, 0 },
                0.0f,
                Color{ 88, 60, 72, 255 }
            );

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
}
