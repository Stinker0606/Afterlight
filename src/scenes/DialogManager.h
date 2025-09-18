#pragma once
#include <string>
#include "raylib.h"

class DialogManager
{
private:
    bool is_active_ = false;
    std::string current_text_;
    std::string character_name_;
    Texture2D portrait_texture_;
    bool has_portrait_ = false;

    Font dialog_font_;

public:
    DialogManager();
    ~DialogManager();

    void ShowDialog(const std::string& text, const std::string& name = "", const std::string& portrait_path = "");
    void HideDialog();
    void Draw();
    bool IsActive() const;
    void DrawTextWithFormatting(const std::string& text, Vector2 pos, float font_size, float spacing, Color default_color) const;
};