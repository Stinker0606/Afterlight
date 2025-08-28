#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include "Store.h"
#include "LevelScene.h"
#include "AssetManager.h"
#include "SoundManager.h"

using namespace std::string_literals;

namespace game::scenes
{
    MenuScene::MenuScene()
    {
        // Starte die Hauptmenü-Musik
        SoundManager::GetInstance().PlayMusic("menu_music");

        // Lade die Assets über den AssetManager
        logo_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/Afterlight_logo.png");
        selector_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/arrow_ui.png");

        // Lade die Schriftart, die bereits im Projekt ist
        menu_font_ = LoadFont("assets/fonts/PixelOperator.ttf");
        // Setze den Texturfilter für die Schriftart auf FILTER_POINT für scharfe Pixel
        SetTextureFilter(menu_font_.texture, TEXTURE_FILTER_POINT);

        // Definiere die Menüpunkte
        menu_items_ = { "Play", "Settings", "Credits", "Quit" };
        selected_item_index_ = 0; // "Play" ist zu Beginn ausgewählt

        // Definiere die Farben (basierend auf deinem Feedback)
        background_color_ = { 0, 44, 56, 255 };      // #002c38
        text_color_ = { 68, 52, 68, 255 };          // #443444
        selected_text_color_ = { 216, 176, 168, 255 }; // #d8b0a8
    }

    MenuScene::~MenuScene()
    {
        // Assets werden vom AssetManager verwaltet und müssen hier nicht entladen werden.
        UnloadFont(menu_font_);
    }

    void MenuScene::Update()
    {
        // Navigation mit Pfeiltasten - angepasst, um nicht im Kreis zu springen
        if (IsKeyPressed(KEY_DOWN))
        {
            if (selected_item_index_ < menu_items_.size() - 1)
            {
                selected_item_index_++;
            }
        }
        if (IsKeyPressed(KEY_UP))
        {
            if (selected_item_index_ > 0)
            {
                selected_item_index_--;
            }
        }

        // Auswahl mit ENTER
        if (IsKeyPressed(KEY_ENTER))
        {
            switch (selected_item_index_)
            {
                case 0: // Play
                    // Wechsle zur Haupt-Spielszene (Level1Scene)
                    game::core::Store::stage->ReplaceWithNewScene("menu"s, "gameplay"s, std::make_unique<Level1Scene>());
                    break;
                case 1: // Settings
                    // Noch keine Funktion
                    break;
                case 2: // Credits
                    // Noch keine Funktion
                    break;
                case 3: // Quit
                    // Schließt die Anwendung. CloseWindow() setzt das Flag, das die Hauptschleife in Game.cpp beendet.
                    CloseWindow();
                    break;
            }
        }
    }

 void MenuScene::Draw()
    {
        ClearBackground(background_color_);

        // --- Logo zeichnen (skaliert und neu positioniert) ---
        float scale = 0.7f; // Skalierungsfaktor für das Logo (kannst du anpassen)
        float logo_width = logo_texture_.width * scale;
        float logo_height = logo_texture_.height * scale;
        float logo_x = (GetScreenWidth() / 2.0f) - (logo_width / 2.0f);
        float logo_y = -180; // Weiter nach oben verschoben

        DrawTexturePro(
            logo_texture_,
            { 0, 0, (float)logo_texture_.width, (float)logo_texture_.height },
            { logo_x, logo_y, logo_width, logo_height },
            { 0, 0 },
            0.0f,
            WHITE
        );

        // --- Menüpunkte zeichnen (größer und neu positioniert) ---
        float font_size = 120; // Schriftgröße erhöht
        int initial_y = 420; // Position nach unten angepasst, um Platz für das Logo zu schaffen
        int spacing = 110;    // Abstand zwischen den Punkten vergrößert

        for (int i = 0; i < menu_items_.size(); ++i)
        {
            Vector2 text_size = MeasureTextEx(menu_font_, menu_items_[i].c_str(), font_size, 2);
            float text_x = (GetScreenWidth() / 2.0f) - (text_size.x / 2.0f);
            float text_y = initial_y + (i * spacing);

            Color current_color = (i == selected_item_index_) ? selected_text_color_ : text_color_;

            // Auswahl-Pfeil zeichnen (skaliert)
            if (i == selected_item_index_)
            {
                float selector_scale = 3.5f; // Skalierungsfaktor für den Pfeil
                float selector_width = selector_texture_.width * selector_scale;
                float selector_height = selector_texture_.height * selector_scale;
                float selector_x = text_x - selector_width - 30; // Abstand angepasst
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
        }
    }
}
