#include <raylib.h>
#include "../scenes/Screen.h"
#include "Game.h"
#include "store.h"
#include "Stage.h"
#include "../scenes/MenuScene.h"

using namespace std::string_literals;

game::core::Game::Game(int stage_Width, int stage_Height, bool full_Screen, int target_Fps, int window_Flags,
                       int texture_Filter, int exit_Key, bool mouse, bool audio, const char *project_Name)
        : stage_Width_(stage_Width), stage_Height_(stage_Height), audio_(audio), mouse_(mouse)
{
    SetConfigFlags(window_Flags);
    InitWindow(stage_Width, stage_Height, project_Name);
    SetWindowMinSize(stage_Width / 2, stage_Height / 2);
    SetTargetFPS(target_Fps);

    // Render texture initialization, used to hold the rendering result, so we can easily resize it
    this->render_Target_ = LoadRenderTexture(this->stage_Width_, this->stage_Height_);
    // Set texture scale filter to use
    SetTextureFilter(this->render_Target_.texture, texture_Filter);

    SetExitKey(exit_Key);

    if (full_Screen)
        ToggleFullscreen();

    if (audio)
        InitAudioDevice();

    if (!mouse)
        HideCursor();
}

game::core::Game::~Game()
{
    TraceLog(LOG_INFO, "game::core::Game destructor called");

    // Release the stage object to trigger potential cleanups that may be dependent on code
    // after the Run() function call.
    game::core::Store::stage = nullptr;

    if (this->audio_)
        CloseAudioDevice();

    // Unload render texture
    UnloadRenderTexture(this->render_Target_);

    // Close window and OpenGL context
    CloseWindow();
}

void game::core::Game::Run(const std::string &scene_Name, std::unique_ptr<game::core::Scene> scene) const
{
    // Create game::Stage instance and assign new scene
    game::core::Store::stage = std::make_unique<game::core::Stage>(scene_Name, std::move(scene));

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button if defined
    {
        if(this->mouse_)
            this->UpdateMousePosition();

        // Process input and update current active scene
        game::core::Store::stage->Update();

        // Draw
        BeginDrawing();

        ClearBackground(BLACK); // Letterbox color

        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(this->render_Target_);
        // Draw the current active scene to render texture
        game::core::Store::stage->Draw();
        EndTextureMode();

        // Draw render texture to window, properly scaled
        this->DrawRenderTexture();
        EndDrawing();

    } // Main game loop end
}

Vector2 game::core::Game::ClampValue(Vector2 value, Vector2 MIN, Vector2 MAX)
{
    Vector2 result = value;

    result.x = (result.x > MAX.x) ? MAX.x : result.x;
    result.x = (result.x < MIN.x) ? MIN.x : result.x;
    result.y = (result.y > MAX.y) ? MAX.y : result.y;
    result.y = (result.y < MIN.y) ? MIN.y : result.y;

    return result;
}

void game::core::Game::UpdateMousePosition() const
{
    // Compute required framebuffer scaling
    float scale = MIN((float) GetScreenWidth() / this->stage_Width_, (float) GetScreenHeight() / this->stage_Height_);

    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 mouse = GetMousePosition();
    Store::mouse_Position.x = (mouse.x - (static_cast<float>(GetScreenWidth()) - (static_cast<float>
        (this->stage_Width_) * scale)) * 0.5f) / scale;
    Store::mouse_Position.y = (mouse.y - (static_cast<float>(GetScreenHeight()) - (static_cast<float>
        (this->stage_Height_) * scale)) * 0.5f) / scale;
    Store::mouse_Position = ClampValue(Store::mouse_Position, {0, 0}, {static_cast<float>
        (this->stage_Width_), static_cast<float>(this->stage_Height_)});
}

void game::core::Game::DrawRenderTexture() const
{
    // Compute required framebuffer scaling
    float scale = MIN((float) GetScreenWidth() / this->stage_Width_, (float) GetScreenHeight() / this->stage_Height_);

    // Draw RenderTexture2D to window, properly scaled
    DrawTexturePro(this->render_Target_.texture,
                   {0.0f, 0.0f, static_cast<float>(this->render_Target_.texture.width),
                       static_cast<float>(-this->render_Target_.texture.height)},
                   {(static_cast<float>(GetScreenWidth()) -
                       (static_cast<float>(this->stage_Width_) * scale))* 0.5f,
                    (static_cast<float>(GetScreenHeight()) - (static_cast<float>
                        (this->stage_Height_) * scale)) * 0.5f,
                    static_cast<float>(this->stage_Width_) * scale, static_cast<float>
                       (this->stage_Height_) * scale},
                   {0.0f, 0.0f}, 0.0f, WHITE
    );
}