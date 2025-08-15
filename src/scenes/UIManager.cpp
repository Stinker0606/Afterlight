#include "UIManager.h"
#include "../game/PlayerClass.h"
#include <string>

UIManager::UIManager()
{
    // Ladet die Grafiken für die UI.
    health_bar_texture_ = LoadTexture("assets/graphics/ui/UI_Trank.png");
    key_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Schlussel.png");
    bomb_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Bombe.png");
    score_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Score.png");
    book_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Book.png");
    blessings_icon_texture_ = LoadTexture("assets/graphics/ui/UI_Segen.png");

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

void UIManager::DrawUI()
{
    if (auto player = player_ptr_.lock())
    {
        // Skalierungsfaktor NUR für HP
        float hp_scale = 2.0f;
        // Skalierungsfaktor für ALLE ANDEREN Icons
        float other_icons_scale = 1.8f;

        // === OBEN LINKS ===

        // Lebensanzeige (Trank)
        Vector2 health_pos = { 10, 10 };
        DrawTextureEx(health_bar_texture_, health_pos, 0.0f, hp_scale, WHITE);
        std::string health_text = std::to_string((int)player->GetHealth());
        DrawTextEx(pixel_font_bold_, health_text.c_str(), { health_pos.x + 40, health_pos.y + 92 }, 30, 1, BLACK);

        // Score
        Vector2 score_pos = { 10, 155 };
        // Zeichne das Score-Icon
        DrawTextureEx(score_icon_texture_, score_pos, 0.0f, hp_scale, WHITE);
        // Zeichne die Score-Zahl daneben
        std::string score_text = std::to_string(8214); // Platzhalter
        DrawTextEx(pixel_font_, score_text.c_str(), { score_pos.x + 71, score_pos.y + 18 }, 28, 1, BLACK);

        // Bomben
        Vector2 bomb_pos = { 120, 20 };
        DrawTextureEx(bomb_icon_texture_, bomb_pos, 0.0f, other_icons_scale, WHITE);
        std::string bomb_text = std::to_string(player->Get_Bomb_Count());
        DrawTextEx(pixel_font_, bomb_text.c_str(), { bomb_pos.x + 55, bomb_pos.y + 15 }, 30, 2, BLACK);

        // Schlüssel
        Vector2 key_pos = { 200, 20 };
        DrawTextureEx(key_icon_texture_, key_pos, 0.0f, other_icons_scale, WHITE);
        std::string key_text = std::to_string(player->Get_Key_Count());
        DrawTextEx(pixel_font_, key_text.c_str(), { key_pos.x + 50, key_pos.y + 15 }, 30, 2, BLACK);

        // === UNTEN LINKS ===

        // Segen (Blessings)
        Vector2 blessings_pos = { 5, (float)GetScreenHeight() - 155 };
        DrawTextureEx(blessings_icon_texture_, blessings_pos, 0.0f, other_icons_scale, WHITE);
        Color blessing_color = { 128, 96, 88, 255 };
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 43, blessings_pos.y + 70 }, 26, 2, blessing_color);
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 156, blessings_pos.y + 70 }, 26, 2, blessing_color);
        DrawTextEx(pixel_font_, "20%", { blessings_pos.x + 269, blessings_pos.y + 70 }, 26, 2, blessing_color);

        // === UNTEN RECHTS ===

        // Buch
        Vector2 book_pos = { (float)GetScreenWidth() - 170, (float)GetScreenHeight() - 110 };
        DrawTextureEx(book_icon_texture_, book_pos, 0.0f, other_icons_scale, WHITE);
    }
}
