#pragma once
#include "raylib.h"
#include <memory>

class PlayerClass;

class UIManager
{
private:
    // Texturen für UI-Elemente
    Texture2D health_full_texture_;
    Texture2D health_medium_texture_;
    Texture2D health_low_texture_;
    Texture2D key_icon_texture_;
    Texture2D bomb_icon_texture_;
    Texture2D score_icon_texture_;
    Texture2D book_icon_texture_;
    Texture2D blessings_icon_texture_;
    Texture2D reticle_texture_;

    // Die benutzerdefinierte Schriftart
    Font pixel_font_;
    Font pixel_font_bold_;

    // Ein schwacher Zeiger auf den Spieler
    std::weak_ptr<PlayerClass> player_ptr_;

public:
    UIManager();
    ~UIManager();

    void SetPlayer(std::shared_ptr<PlayerClass> player);
    void DrawUI(Camera2D camera);
};