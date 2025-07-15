#include "GameScene.h"
#include <memory>
#include <string>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <stdio.h>
#include "Screen.h"
#include <Store.h>
#include "PauseScene.h"
#include "config.h"
#include "raymath.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene() {
    dtm.Start();
    key_cooldown = 0.0f;
    objectManager.AddObject(&mp);
    cam = std::make_shared<Cam>(this->mp);
    screen.LoadGameObjects(objectManager);
}

game::scenes::GameScene::~GameScene() {}

void game::scenes::GameScene::Update() {
    float dt = dtm.Get_Dt();
    Handle_Fog_Controls(dt);

    for (auto& obj : objectManager.managed_objects) {
        obj->Tick(dt);
    }



    p_cm->Check_Collisions();
    cam->Cam_Movement(dt);

    Vector2 player_world_pos = cam->cam.target;
    Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, cam->cam);
    Vector2 fog_visual_offset = { -88.0f, -69.0f };
    Vector2 final_fog_pos = Vector2Add(player_screen_pos, fog_visual_offset);
    screen.UpdateFog(final_fog_pos, dt);

    objectManager.Cleanup_Objects();
    dtm.Update();
}

// Draw-Funktion, die dem Stand entspricht, bei dem der Nebel sichtbar war
void game::scenes::GameScene::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    // --- 1. Zeichne die gesamte Spielwelt normal (ohne Nebel) ---
    bool originalFogState = screen.fogManager.fogEnabled;
    screen.fogManager.fogEnabled = false;

    screen.Draw_Level(this->cam, false);
    BeginMode2D(cam->cam);
    for (const auto& p_object : objectManager.managed_objects) {
        if (p_object != nullptr) {
            p_object->Draw();
        }
    }

    for (const auto& p_object : objectManager.managed_objects)
    {
        if (p_object != nullptr)
        {
            DrawRectangleLinesEx(p_object->Get_Hitbox(), 2.0f, RED);
        }
    }

    EndMode2D();
    screen.Draw_Level(this->cam, true);

    screen.fogManager.fogEnabled = originalFogState;

    // --- 2. Zeichne den durchsichtigen Nebel als Overlay ---
    if (screen.fogManager.fogEnabled)
    {
        screen.fogManager.BeginFogMode();
        // Zeichnet ein durchsichtiges Rechteck, auf das der Shader angewendet wird
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLANK);
        screen.fogManager.EndFogMode();
    }

    // --- 3. Zeichne die UI ganz oben ---
    Draw_Fog_UI();

}

// Die Steuerungs- und UI-Funktionen bleiben unverändert
void game::scenes::GameScene::Handle_Fog_Controls(float delta_time) {
    if (key_cooldown > 0) { key_cooldown -= delta_time; }
    if (key_cooldown <= 0) {
        bool key_pressed = false;
        if (IsKeyPressed(KEY_L)) { ToggleFullscreen(); }
        if (IsKeyPressed(KEY_F)) { screen.fogManager.fogEnabled = !screen.fogManager.fogEnabled; }
        if (IsKeyDown(KEY_UP)) { screen.fogManager.outerRadius += 5.0f; key_pressed = true; }
        if (IsKeyDown(KEY_DOWN)) { screen.fogManager.outerRadius -= 5.0f; key_pressed = true; }
        if (IsKeyDown(KEY_RIGHT)) { screen.fogManager.innerRadius += 5.0f; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT)) { screen.fogManager.innerRadius -= 5.0f; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_R)) { screen.fogManager.fogColor.r -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_R)) { screen.fogManager.fogColor.r += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_G)) { screen.fogManager.fogColor.g -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_G)) { screen.fogManager.fogColor.g += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_B)) { screen.fogManager.fogColor.b -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_B)) { screen.fogManager.fogColor.b += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Q)) { screen.fogManager.fogColor.a -= 5; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_Q)) { screen.fogManager.fogColor.a += 5; key_pressed = true; }
        screen.fogManager.outerRadius = Clamp(screen.fogManager.outerRadius, 150, 300);
        screen.fogManager.innerRadius = Clamp(screen.fogManager.innerRadius, 40, screen.fogManager.outerRadius - 100);
        screen.fogManager.fogColor.r = Clamp(screen.fogManager.fogColor.r, 145, 165);
        screen.fogManager.fogColor.g = Clamp(screen.fogManager.fogColor.g, 145, 165);
        screen.fogManager.fogColor.b = Clamp(screen.fogManager.fogColor.b, 145, 165);
        screen.fogManager.fogColor.a = Clamp(screen.fogManager.fogColor.a, 180, 240);
        if(key_pressed) { key_cooldown = KEY_PRESS_DELAY; }
    }
}

void game::scenes::GameScene::Draw_Fog_UI() {
    char text_buffer[256];
    int y_pos = 10;
    int line_height = 25;
    DrawText("--- Nebel-Steuerung ---", 10, y_pos, 20, RAYWHITE); y_pos += line_height;
    DrawText("F: Nebel An/Aus", 10, y_pos, 20, (screen.fogManager.fogEnabled ? GREEN : RED)); y_pos += line_height;
    sprintf(text_buffer, "Pfeiltasten: L/R Inner Radius | U/D Outer Radius (Inner: %.0f, Outer: %.0f)", screen.fogManager.innerRadius, screen.fogManager.outerRadius);
    DrawText(text_buffer, 10, y_pos, 20, SKYBLUE); y_pos += line_height;
    sprintf(text_buffer, "R/G/B/Q (-Shift) oder (+CTL): Farbe (RGB) & Stärke(Q)");
    DrawText(text_buffer, 10, y_pos, 20, SKYBLUE); y_pos += line_height;
    sprintf(text_buffer, "Werte (RGBA): %d, %d, %d, %d", screen.fogManager.fogColor.r, screen.fogManager.fogColor.g, screen.fogManager.fogColor.b, screen.fogManager.fogColor.a);
    DrawText(text_buffer, 10, y_pos, 20, LIME);
}