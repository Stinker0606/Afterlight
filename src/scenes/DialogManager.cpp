#include "DialogManager.h"
#include "AssetManager.h"
#include "../config.h.in"

DialogManager::DialogManager()
{
    dialog_font_ = LoadFontEx("assets/fonts/alagard.ttf", 250, nullptr, 0);
    SetTextureFilter(dialog_font_.texture, TEXTURE_FILTER_POINT);
}

DialogManager::~DialogManager()
{
    UnloadFont(dialog_font_);
}

void DialogManager::ShowDialog(const std::string& text, const std::string& name, const std::string& portrait_path)
{
    current_text_ = text;
    character_name_ = name;

    if (!portrait_path.empty()) {
        portrait_texture_ = AssetManager::GetInstance().Load(portrait_path.c_str());
        has_portrait_ = true;
    } else {
        has_portrait_ = false;
    }

    is_active_ = true;
}

void DialogManager::HideDialog()
{
    is_active_ = false;
}

bool DialogManager::IsActive() const
{
    return is_active_;
}

void DialogManager::Draw()
{
    if (!is_active_) return;

    // --- Box-Positionierung zwischen den UI-Elementen ---
    float box_height = 220;
    float box_y = game::Config::kStageHeight - box_height - 20;
    Rectangle box_rect = { 350, box_y, (float)game::Config::kStageWidth - 700, box_height };

    // --- Box zeichnen ---
    DrawRectangleRec(box_rect, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(box_rect, 3, Fade(WHITE, 0.6f));

    // --- Porträt (rechts) ---
    float text_start_x = box_rect.x + 30;
    float text_end_x = box_rect.x + box_rect.width - 30;
    if (has_portrait_) {
        float portrait_size = box_height - 40;
        DrawTextureEx(portrait_texture_, {box_rect.x + box_rect.width - portrait_size - 20, box_rect.y + 20}, 0.0f, portrait_size / portrait_texture_.height, WHITE);
        text_end_x -= (portrait_size + 40);
    }

    // --- Name (falls vorhanden) ---
    if (!character_name_.empty()) {
        DrawTextEx(dialog_font_, character_name_.c_str(), {text_start_x, box_rect.y + 20}, 50, 2, Color{ 216, 176, 168, 255 });
        DrawTextEx(dialog_font_, current_text_.c_str(), {text_start_x, box_rect.y + 90}, 25, 2, Fade(WHITE, 0.9f));
    } else {
        DrawTextEx(dialog_font_, current_text_.c_str(), {text_start_x, box_rect.y + 40}, 30, 2, Fade(WHITE, 0.9f));
    }

}