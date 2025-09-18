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
    DrawRectangleRec(box_rect, Fade(BLACK, 0.6f));
    DrawRectangleLinesEx(box_rect, 3, Fade(WHITE, 0.6f));

    // --- Porträt (rechts) ---
    float text_start_x = box_rect.x + 30;
    float text_end_x = box_rect.x + box_rect.width - 30;
    if (has_portrait_) {
        float portrait_size = box_height + 45;
        DrawTextureEx(portrait_texture_, {box_rect.x + box_rect.width - portrait_size - 20, box_rect.y - 60}, 0.0f, portrait_size / portrait_texture_.height, WHITE);
        text_end_x -= (portrait_size + 40);
    }

    // --- Name (falls vorhanden) ---
    if (!character_name_.empty()) {
        DrawTextEx(dialog_font_, character_name_.c_str(), {text_start_x, box_rect.y + 20}, 50, 2, Color{ 216, 176, 168, 255 });
        DrawTextWithFormatting(current_text_, {text_start_x, box_rect.y + 90}, 25, 2, Fade(WHITE, 0.9f));
    } else {
        DrawTextWithFormatting(current_text_, {text_start_x, box_rect.y + 40}, 30, 2, Fade(WHITE, 0.9f));
    }

}

void DialogManager::DrawTextWithFormatting(const std::string& text, Vector2 pos, float font_size, float spacing, Color default_color) const {
    std::string buffer;
    bool bold = false;
    Color current_color = default_color;
    float x = pos.x;
    float y = pos.y;

    auto Flush = [&](bool bold_state, Color col) {
        if (!buffer.empty()) {
            float size = bold_state ? font_size * 1.2f : font_size;

            // --- Baseline-Korrektur ---
            Vector2 normal = MeasureTextEx(dialog_font_, "A", font_size, spacing);
            Vector2 boldM  = MeasureTextEx(dialog_font_, "A", size, spacing);
            float y_offset = bold_state ? (normal.y - boldM.y) / 2.0f : 0.0f;

            DrawTextEx(dialog_font_, buffer.c_str(), {x, y + y_offset}, size, spacing, col);
            Vector2 text_size = MeasureTextEx(dialog_font_, buffer.c_str(), size, spacing);
            x += text_size.x;
            buffer.clear();
        }
    };

    for (size_t i = 0; i < text.size(); ++i) {
        // --- Zeilenumbruch ---
        if (text[i] == '\n') {
            Flush(bold, current_color);
            x = pos.x;
            y += font_size + 10;
            continue;
        }

        // --- <b> ---
        if (i + 2 < text.size() && text.substr(i, 3) == "<b>") {
            Flush(bold, current_color);
            bold = true;
            i += 2;
            continue;
        }
        // --- </b> ---
        if (i + 3 < text.size() && text.substr(i, 4) == "</b>") {
            Flush(bold, current_color);
            bold = false;
            i += 3;
            continue;
        }

        // --- <brown> ---
        if (i + 6 < text.size() && text.substr(i, 7) == "<brown>") {
            Flush(bold, current_color);
            current_color = {128, 96, 88, 255}; // your brown
            i += 6;
            continue;
        }
        // --- </brown> (reset) ---
        if (i + 7 < text.size() && text.substr(i, 8) == "</brown>") {
            Flush(bold, current_color);
            current_color = default_color;
            i += 7;
            continue;
        }

        // --- <white> ---
        if (i + 6 < text.size() && text.substr(i, 7) == "<white>") {
            Flush(bold, current_color);
            current_color = {196, 172, 156, 255}; // your white
            i += 6;
            continue;
        }
        // --- </white> (reset) ---
        if (i + 7 < text.size() && text.substr(i, 8) == "</white>") {
            Flush(bold, current_color);
            current_color = default_color;
            i += 7;
            continue;
        }

        // --- Normales Zeichen ---
        buffer.push_back(text[i]);
    }

    Flush(bold, current_color);
}

