#include "UIManager.h"
#include "../game/PlayerClass.h"
#include <string>
#include "Store.h"
#include "../config.h.in"
#include "raymath.h"

UIManager::UIManager()
{
    // Ladet die Grafiken für die UI.
    health_bar_texture_ = LoadTexture("assets/graphics/ui/UI_Trank.png");
    key_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Schlussel.png");
    bomb_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Bombe.png");
    score_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Score.png");
    book_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Book.png");
    blessings_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Segen.png");

    // Fadenkreuz des Players
    reticle_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/reticle2.png");

    // Ladet benutzerdefinierte Schriftart.
    pixel_font_ = LoadFont("assets/fonts/PixelOperator.ttf");
    pixel_font_bold_ = LoadFont("assets/fonts/PixelOperator-Bold.ttf");
}

UIManager::~UIManager()
{
    // Entlade alle Texturen und die Schriftart
    UnloadTexture(health_bar_texture_);
    UnloadTexture(key_icon_texture_);
    UnloadTexture(bomb_icon_texture_);
    UnloadTexture(score_icon_texture_);
    UnloadTexture(book_icon_texture_);
    UnloadTexture(blessings_icon_texture_);
    UnloadFont(pixel_font_);
}

void UIManager::SetPlayer(std::shared_ptr<PlayerClass> player)
{
    player_ptr_ = player;
}

void UIManager::DrawUI(Camera2D camera)
{
    if (auto player = player_ptr_.lock())
    {
        // --- Konfiguration ---
        Color outline_color = { 108, 112, 108, 255 }; // Dein Farbcode #6c706c
        float text_offset = 1.2f; // Die "Dicke" der Outline in Pixeln

        // Skalierungsfaktoren
        float hp_scale = 2.0f;
        float icons_scale = 1.7f;
        float other_icons_scale = 1.8f;

        // --- Helfer-Funktion für Text mit Outline ---
        // Diese Funktion zeichnet den Text viermal versetzt in der Outline-Farbe
        // und dann einmal normal in der Textfarbe darüber.
        auto draw_text_with_outline = [&](const Font& font, const std::string& text, Vector2 pos, float fontSize, float spacing, Color text_color) {
            DrawTextEx(font, text.c_str(), { pos.x - text_offset, pos.y }, fontSize, spacing, outline_color);
            DrawTextEx(font, text.c_str(), { pos.x + text_offset, pos.y }, fontSize, spacing, outline_color);
            DrawTextEx(font, text.c_str(), { pos.x, pos.y - text_offset }, fontSize, spacing, outline_color);
            DrawTextEx(font, text.c_str(), { pos.x, pos.y + text_offset }, fontSize, spacing, outline_color);
            DrawTextEx(font, text.c_str(), pos, fontSize, spacing, text_color);
        };

        // === OBEN LINKS ===

        // Lebensanzeige
        Vector2 health_pos = { 10, 10 };
        DrawTextureEx(health_bar_texture_, health_pos, 0.0f, hp_scale, WHITE);
        std::string health_text = std::to_string((int)player->GetHealth());
        draw_text_with_outline(pixel_font_bold_, health_text, { health_pos.x + 30, health_pos.y + 85 }, 30, 1, BLACK);

        // Score
        Vector2 score_pos = { 10, 155 };
        DrawTextureEx(score_icon_texture_, score_pos, 0.0f, hp_scale, WHITE);
        std::string score_text = std::to_string(player->Get_Score());
        draw_text_with_outline(pixel_font_, score_text, { score_pos.x + 55, score_pos.y + 5 }, 28, 1, BLACK);

        // Bomben
        Vector2 bomb_pos = { 120, 20 };
        DrawTextureEx(bomb_icon_texture_, bomb_pos, 0.0f, other_icons_scale, WHITE);
        std::string bomb_text = std::to_string(player->Get_Bomb_Count());
        draw_text_with_outline(pixel_font_, bomb_text, { bomb_pos.x + 45, bomb_pos.y + 15 }, 30, 2, BLACK);

        // Schlüssel
        Vector2 key_pos = { 200, 20 };
        DrawTextureEx(key_icon_texture_, key_pos, 0.0f, other_icons_scale, WHITE);
        std::string key_text = std::to_string(player->Get_Key_Count());
        draw_text_with_outline(pixel_font_, key_text, { key_pos.x + 45, key_pos.y + 15 }, 30, 2, BLACK);

        // === UNTEN LINKS ===

        // Segen (Blessings)
        Vector2 blessings_pos = { 10, (float)GetScreenHeight() - 150 };
        DrawTextureEx(blessings_icon_texture_, blessings_pos, 0.0f, icons_scale, WHITE);
        Color blessing_color = { 128, 96, 88, 255 };
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 30, blessings_pos.y + 45 }, 25, 2, blessing_color);
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 134, blessings_pos.y + 45 }, 25, 2, blessing_color);
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 242, blessings_pos.y + 45 }, 25, 2, blessing_color);

        // === UNTEN RECHTS ===

        // Buch
        Vector2 book_pos = { (float)GetScreenWidth() - 190, (float)GetScreenHeight() - 115 };
        DrawTextureEx(book_icon_texture_, book_pos, 0.0f, hp_scale, WHITE);

        // === Fadenkreuz ===

        // 1. Positionen holen
        Vector2 mouse_pos = game::core::Store::mouse_Position;
        Vector2 reticle_pos = mouse_pos;

        // 2. Fadenkreuz an der berechneten Position zeichnen
        // Wir ziehen die halbe Breite/Höhe ab, damit es zentriert ist
        reticle_pos.x -= reticle_texture_.width / 2.0f;
        reticle_pos.y -= reticle_texture_.height / 2.0f;
        DrawTextureV(reticle_texture_, reticle_pos, (Color){ 88, 60, 72, 255 });
    }
}