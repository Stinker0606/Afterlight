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

game::scenes::GameScene::GameScene() {
    dtm.Start();
    key_cooldown = 0.0f;
    objectManager.AddObject(&mp);
    cam = std::make_shared<Cam>(this->mp);
    screen.LoadGameObjects(objectManager);
    screen.fogManager.InitializeFog("", {(float)game::Config::kStageWidth, (float)game::Config::kStageHeight});
}

game::scenes::GameScene::~GameScene() {}

void game::scenes::GameScene::Update() {
    float dt = dtm.Get_Dt();
    Handle_Fog_Controls(dt);
    for (auto& obj : objectManager.managed_objects) { obj->Tick(dt); }
    p_cm->Check_Collisions();
    cam->Cam_Movement(dt);
    Vector2 player_world_pos = cam->cam.target;
    Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, cam->cam);
    screen.UpdateFog(player_screen_pos, dt);
    objectManager.Cleanup_Objects();
    dtm.Update();
}

// Die FINALE, KORREKTE Draw-Funktion
void game::scenes::GameScene::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    // KORREKTUR: Zuerst die Kamera starten, dann den Shader anwenden.
    BeginMode2D(cam->cam);

        screen.fogManager.BeginFogMode();

            // Zeichne alle Spielwelt-Elemente, die vom Nebel betroffen sein sollen
            screen.Draw_Level(this->cam, false);
            mp.Draw();
            // screen.Draw_Level(this->cam, true); // Falls du Ebenen über dem Spieler hast

        screen.fogManager.EndFogMode();

    EndMode2D();

    // Zeichne die UI ganz zum Schluss, damit sie immer sichtbar ist.
    Draw_Fog_UI();

}

// Die Steuerungs- und UI-Funktionen bleiben unverändert
void game::scenes::GameScene::Handle_Fog_Controls(float delta_time) {
    if (key_cooldown > 0) { key_cooldown -= delta_time; }
    if (key_cooldown <= 0) {
        bool key_pressed = false;
        if (IsKeyPressed(KEY_F)) { screen.fogManager.fogEnabled = !screen.fogManager.fogEnabled; }
        if (IsKeyDown(KEY_UP)) { screen.fogManager.outerRadius += 1.0f; key_pressed = true; }
        if (IsKeyDown(KEY_DOWN)) { screen.fogManager.outerRadius -= 1.0f; key_pressed = true; }
        if (IsKeyDown(KEY_RIGHT)) { screen.fogManager.innerRadius += 1.0f; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT)) { screen.fogManager.innerRadius -= 1.0f; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_R)) { screen.fogManager.fogColor.r -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_R)) { screen.fogManager.fogColor.r += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_G)) { screen.fogManager.fogColor.g -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_G)) { screen.fogManager.fogColor.g += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_B)) { screen.fogManager.fogColor.b -= 1; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_B)) { screen.fogManager.fogColor.b += 1; key_pressed = true; }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Q)) { screen.fogManager.fogColor.a -= 5; key_pressed = true; } else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_Q)) { screen.fogManager.fogColor.a += 5; key_pressed = true; }
        screen.fogManager.outerRadius = Clamp(screen.fogManager.outerRadius, 0, 1000);
        screen.fogManager.innerRadius = Clamp(screen.fogManager.innerRadius, 0, screen.fogManager.outerRadius - 1);
        screen.fogManager.fogColor.r = Clamp(screen.fogManager.fogColor.r, 0, 255);
        screen.fogManager.fogColor.g = Clamp(screen.fogManager.fogColor.g, 0, 255);
        screen.fogManager.fogColor.b = Clamp(screen.fogManager.fogColor.b, 0, 255);
        screen.fogManager.fogColor.a = Clamp(screen.fogManager.fogColor.a, 140, 255);
        if(key_pressed) { key_cooldown = KEY_PRESS_DELAY; }
    }
}

void game::scenes::GameScene::Draw_Fog_UI() {
    char text_buffer[256];
    int y_pos = 10;
    int line_height = 25;
    DrawText("--- Nebel-Steuerung ---", 10, y_pos, 20, RAYWHITE); y_pos += line_height;
    DrawText("F: Nebel An/Aus", 10, y_pos, 20, (screen.fogManager.fogEnabled ? GREEN : RED)); y_pos += line_height;
    sprintf(text_buffer, "Pfeiltasten: Radius (Inner: %.0f, Outer: %.0f)", screen.fogManager.innerRadius, screen.fogManager.outerRadius);
    DrawText(text_buffer, 10, y_pos, 20, SKYBLUE); y_pos += line_height;
    sprintf(text_buffer, "R/G/B/Q (+Shift) oder (+CTL): Farbe & Stärke");
    DrawText(text_buffer, 10, y_pos, 20, SKYBLUE); y_pos += line_height;
    sprintf(text_buffer, "Werte (RGBA): %d, %d, %d, %d", screen.fogManager.fogColor.r, screen.fogManager.fogColor.g, screen.fogManager.fogColor.b, screen.fogManager.fogColor.a);
    DrawText(text_buffer, 10, y_pos, 20, LIME);
}