#include "SplashScreenScene.h"
#include "Store.h"
#include "MenuScene.h"
#include "AssetManager.h"
#include "../config.h.in"

namespace game::scenes
{
    SplashScreenScene::SplashScreenScene()
    {
        logo_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/Afterlight_logo.png");
        current_state_ = State::FADE_IN;
        timer_ = 0.0f;
        alpha_ = 0.0f;
    }

    SplashScreenScene::~SplashScreenScene()
    {
        // Textur wird vom AssetManager verwaltet
    }

    void SplashScreenScene::Update()
    {
        timer_ += GetFrameTime();

        switch (current_state_)
        {
            case State::FADE_IN:
                alpha_ += GetFrameTime() / 1.5f; // Fade-In über 1.5 Sekunden
            if (alpha_ >= 1.0f) {
                alpha_ = 1.0f;
                current_state_ = State::HOLD;
                timer_ = 0.0f;
            }
            break;

            case State::HOLD:
                if (timer_ >= 2.0f) { // Logo 2 Sekunden halten
                    current_state_ = State::FADE_OUT;
                }
            break;

            case State::FADE_OUT:
                alpha_ -= GetFrameTime() / 1.5f; // Fade-Out über 1.5 Sekunden
            if (alpha_ <= 0.0f) {
                alpha_ = 0.0f;
                current_state_ = State::FINISHED;
            }
            break;

            case State::FINISHED:
            {
                // Erstelle die neue Szene
                auto menu_scene = std::make_unique<MenuScene>();
                // Rufe die neue Funktion auf, um das Fade-In zu starten
                menu_scene->TriggerFadeIn();
                // Übergebe die vorbereitete Szene an den Stage-Manager
                game::core::Store::stage->ReplaceWithNewScene("splash", "menu", std::move(menu_scene));
                break;
            }
        }
    }

    void SplashScreenScene::Draw()
    {
        // 1. Zeichne immer den gewünschten farbigen Hintergrund
        ClearBackground(Color{ 0, 44, 56, 255 });

        // 2. Zeichne das Logo immer mit voller Deckkraft
        float scale = 0.8f;
        float logo_width = logo_texture_.width * scale;
        float logo_height = logo_texture_.height * scale;
        float logo_x = (game::Config::kStageWidth / 2.0f) - (logo_width / 2.0f);
        float logo_y = (game::Config::kStageHeight / 2.0f) - (logo_height / 2.0f) - 100;
        DrawTextureEx(logo_texture_, {logo_x, logo_y}, 0.0f, scale, WHITE);

        // 3. Zeichne eine schwarze Ebene darüber, deren Transparenz sich ändert.
        float fade_overlay_alpha = 1.0f - alpha_;
        DrawRectangle(0, 0, game::Config::kStageWidth, game::Config::kStageHeight, Fade(BLACK, fade_overlay_alpha));
    }
}