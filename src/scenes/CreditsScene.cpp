#include "CreditsScene.h"
#include "MenuScene.h"
#include "Store.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../config.h.in"

namespace game::scenes
{
    CreditsScene::CreditsScene()
    {
        menuFont_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
        SetTextureFilter(menuFont_.texture, TEXTURE_FILTER_POINT);

        //Hintergrund laden
        background_credits_ = AssetManager::GetInstance().Load("assets/graphics/backgrounds/UI/Background_Credit.png");
        UI_Backspace_ = AssetManager::GetInstance().Load("assets/graphics/ui/Space_UITrig.png");

        // Beispiel-Credits
        // Credits definieren mit eigener Schriftgröße
        creditsLines_ = {
            {"LIGHT", big_font_size_},
            {""},
            {"RESTORED", big_font_size_},
            {"", default_font_size_},
            {""},
            {"AFTERLIGHT", big_font_size_},  // z. B. noch größer als default
            {"", default_font_size_},
            {"", default_font_size_},
            {"Developed by", 90},
            {"", default_font_size_},
            {"Ubifrog", 75},
            {"NPC 1: Andreas", default_font_size_},
            {"NPC 2: Mad", default_font_size_},
            {"NPC 3: Nils", default_font_size_},
            {"NPC 4: Sky", default_font_size_},
            {"NPC 5: Lilli", default_font_size_},
            {"NPC 6: Felix", default_font_size_},
            {"NPC 7: Eule", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Programming", 90},
            {"Andreas", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Level Design", 90},
            {"Mad", default_font_size_},
            {"Nils", default_font_size_},
            {"Sky", default_font_size_},
            {"", default_font_size_},
            {"Narrative Design", 75},
            {"Nils", default_font_size_},
            {"", default_font_size_},
            {"Sound Design", 75},
            {"Mad", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Art & Animation ", 90},
            {"Lilli", default_font_size_},
            {"Eule", default_font_size_},
            {"Felix", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Trailer", 90},
            {"Felix", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Special Thanks to", 90},
            {"", default_font_size_},
            {"Our Professors", default_font_size_},
            {"All Playtesters", default_font_size_},
            {"", default_font_size_},
            {"", default_font_size_},
            {"Thank you for playing!", 80}  // etwas größer für den Schluss
        };

        //Gesamthöhe aller Zeilen berechnen
        totalHeight_ = (float)creditsLines_.size() * 80;
        scrollY_ = game::Config::kStageHeight; // Start unten
    }

    CreditsScene::~CreditsScene()
    {
        UnloadFont(menuFont_);
    }

    void CreditsScene::Update()
    {
        time_ += GetFrameTime();

        // Automatisches Hochscrollen
        scrollY_ -= 50 * GetFrameTime(); // 50px pro Sekunde

        // Endlosschleife: wenn alles oben weg ist, wieder unten starten
        if (scrollY_ + totalHeight_ < 0)
        {
            scrollY_ = game::Config::kStageHeight;
        }

        // Mit ESC zurück ins Menü
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            SoundManager::GetInstance().PlaySfx("ui_select");
            game::core::Store::stage->ReplaceWithNewScene("credits", "menu", std::make_unique<MenuScene>());
        }
    }

    void CreditsScene::Draw()
    {
        // Hintergrund zeichnen
        DrawTextureEx(background_credits_, {0, 0}, 0.0f, 11.55f, WHITE);

        //Backspace UI Element zeichnen
        DrawTextureEx(UI_Backspace_, {game::Config::kStageWidth/ 1.09, 1000}, 0.0f, 2.1f, WHITE);

        // --- Berechnungen für Animationen ---
        float pulse = sin(time_ * 0.8f) * 0.5f + 0.5f;
        float float_offset = sin(time_ * 0.75f) * 10.0f;

        // Animation Größe
        float anim_scale = 25.0f;
        float anim_width = 64.0f * anim_scale;
        float anim_height = 96.0f * anim_scale;

        // Position der Animation
        Vector2 anim_pos = {
            (game::Config::kStageWidth / 2.0f),
            (game::Config::kStageHeight / 10.0f)
        };

        // --- Pulsierender Licht/Flammen-Effekt 1 (oben) ---
        Vector2 light_center = anim_pos;
        float base_radius = 75.0f;
        float light_radius = (base_radius * anim_scale / 6.0f);
        Color flame_inner = { 196, 172, 156, 255 };
        //Color flame_outer = { 0x44, 0x34, 0x44, 255 };
        //DrawCircleGradient(light_center.x, light_center.y, light_radius * 1.5f, ColorAlpha(flame_outer, 0.7f * (pulse * 0.5f + 0.5f)), BLANK);
        DrawCircleGradient(light_center.x, light_center.y, light_radius, ColorAlpha(flame_inner, 0.9f * (pulse * 0.7f + 0.3f)), BLANK);

        // Position der 2. Animation
        Vector2 anim_2_pos = {
            (game::Config::kStageWidth / 2.0f),
            (game::Config::kStageHeight / 1.7f)
        };

        // --- Pulsierender Licht/Flammen-Effekt 2 (unten)---
        Vector2 light_2_center = anim_2_pos;
        float base_2_radius = 150.0f;
        float light_2_radius = (base_2_radius * anim_scale / 6.0f) + pulse * 100.0f;
        Color flame_2_inner = { 196, 172, 156, 255 };
        Color flame_2_outer = { 164, 148, 132, 255 };
        DrawCircleGradient(light_center.x, light_center.y, light_radius * 1.5f, ColorAlpha(flame_2_outer, 0.7f * (pulse * 0.5f + 0.5f)), BLANK);
        DrawCircleGradient(light_center.x, light_2_center.y, light_2_radius, ColorAlpha(flame_2_inner, 0.9f * (pulse * 0.7f + 0.3f)), BLANK);


        // Credits zeichnen
        // Credits zeichnen
        float y = scrollY_;
        for (const auto& line : creditsLines_)
        {
            Vector2 size = MeasureTextEx(menuFont_, line.text_.c_str(), line.font_size_, 2);
            float x = (game::Config::kStageWidth - size.x) / 2.0f;
            DrawTextEx(menuFont_, line.text_.c_str(), {x, y}, line.font_size_, 2, WHITE);
            y += 80;
        }

    }
}
