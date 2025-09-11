#include "SplashScreenScene.h"
#include "Store.h"
#include "MenuScene.h"
#include "AssetManager.h"
#include "../config.h.in"

namespace game::scenes
{
    SplashScreenScene::SplashScreenScene()
    {
        logo_texture_ = AssetManager::GetInstance().Load("assets/graphics/ui/Afterlight_logo_glow.png");
        current_state_ = State::FADE_IN;
        timer_ = 0.0f;
        alpha_ = 1.0f;
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
                // Alpha geht von 1.0 (schwarz) runter auf 0.0 (sichtbar)
                    alpha_ -= GetFrameTime() / 3.0f;
            if (alpha_ <= 0.0f) {
                alpha_ = 0.0f;
                current_state_ = State::HOLD;
                timer_ = 0.0f;
            }
            break;

            case State::HOLD:
                if (timer_ >= 2.0f) {
                    current_state_ = State::FADE_OUT;
                }
            break;

            case State::FADE_OUT:
                // Alpha geht von 0.0 (sichtbar) hoch auf 1.0 (schwarz)
                    alpha_ += GetFrameTime() / 1.5f;
            if (alpha_ >= 1.0f) {
                alpha_ = 1.0f;
                current_state_ = State::FINISHED;
            }
            break;

            case State::FINISHED:
            {
                auto menu_scene = std::make_unique<MenuScene>();
                menu_scene->TriggerFadeIn();
                game::core::Store::stage->ReplaceWithNewScene("splash", "menu", std::move(menu_scene));
                break;
            }
        }
    }

    void SplashScreenScene::Draw()
    {
        // 1. Zeichne immer den gewünschten farbigen Hintergrund
        ClearBackground(Color{ 0, 44, 56, 255 });

        // --- Berechnungen für Animationen ---
        float pulse = sin(timer_ * 0.8f) * 0.5f + 0.5f;
        float float_offset = sin(timer_ * 0.75f) * 10.0f;

        // Animation Größe
        float anim_scale = 25.0f;
        float anim_width = 64.0f * anim_scale;
        float anim_height = 96.0f * anim_scale;

        // Position der Animation
        Vector2 anim_pos = {
            (game::Config::kStageWidth / 2.0f),
            (game::Config::kStageHeight / 2.0f)
        };

        // --- Pulsierender Licht/Flammen-Effekt ---
        Vector2 light_center = anim_pos;
        float base_radius = 180.0f;
        float light_radius = (base_radius * anim_scale / 6.0f) + pulse * 100.0f;
        Color flame_inner = { 0x58, 0x3c, 0x48, 255 };
        Color flame_outer = { 0x44, 0x34, 0x44, 255 };
        DrawCircleGradient(light_center.x, light_center.y, light_radius * 1.5f, ColorAlpha(flame_outer, 0.7f * (pulse * 0.5f + 0.5f)), BLANK);
        DrawCircleGradient(light_center.x, light_center.y, light_radius, ColorAlpha(flame_inner, 0.9f * (pulse * 0.7f + 0.3f)), BLANK);

        // 2. Zeichne das Logo immer mit voller Deckkraft
        float scale = 0.7f;
        float logo_width = logo_texture_.width * scale;
        float logo_height = logo_texture_.height * scale;
        float logo_x = (game::Config::kStageWidth / 2.0f) - (logo_width / 2.0f);
        float logo_y = (game::Config::kStageHeight / 2.0f) - (logo_height / 2.0f) - 50;
        DrawTextureEx(logo_texture_, {logo_x, logo_y}, 0.0f, scale, WHITE);

        // 3. Zeichne die schwarze Überblendung darüber.
        DrawRectangle(0, 0, game::Config::kStageWidth, game::Config::kStageHeight, Fade(BLACK, alpha_));
    }
}