#include "GameScene.h"
#include <memory>
#include <string>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include "Screen.h"
#include <Store.h>
#include "PauseScene.h"
#include "Renderer.h"
#include "SpriteAnimated.h"

#include "../game/PlayerClassOne.h"
#include "../core/CollisionManager.h"
#include "config.h"
#include "raymath.h"

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    dtm.Start();
    this->sp_mp=std::make_shared<Player_Class_One>(sp,objectManager);
    objectManager.AddObject(sp_mp);
    cam=std::make_shared<Cam>(sp_mp);
    screen.LoadGameObjects(objectManager);

    cam->Cam_Movement(0.0f);
    Vector2 initial_player_world_pos = cam->cam.target;
    Vector2 initial_player_screen_pos = GetWorldToScreen2D(initial_player_world_pos, cam->cam);
    screen.UpdateFog(initial_player_screen_pos, 0.0f);
}

game::scenes::GameScene::~GameScene()
{
    // Your scene cleanup code here...
}

void game::scenes::GameScene::Update()
{
    // Your process input and update game scene code here...
    if (IsKeyPressed(KEY_ESCAPE))
        game::core::Store::stage->SwitchToNewScene("pause"s, std::make_unique<PauseScene>());
    if (IsKeyPressed(KEY_L)){
        ToggleFullscreen();
    }

    for (auto& obj : objectManager.managed_objects) {
        obj->Tick(dtm.Get_Dt());
    }

    p_cm->Check_Collisions();
    cam->Cam_Movement(dtm.Get_Dt());

    Vector2 player_world_pos = cam->cam.target;
    Vector2 player_screen_pos = GetWorldToScreen2D(player_world_pos, cam->cam);

    // 1. Definiere hier einen visuellen Offset.
    Vector2 fog_visual_offset = { -0.0f, -0.0f };

    // 2. Addiere den Offset zur berechneten Bildschirm-Position.
    Vector2 final_fog_pos = Vector2Add(player_screen_pos, fog_visual_offset);

    // 3. Übergib die finale, korrigierte Position an den Nebel.
    screen.UpdateFog(final_fog_pos, dtm.Get_Dt());

    objectManager.Cleanup_Objects();
    dtm.Update();
}

void game::scenes::GameScene::Draw()
{
    BeginDrawing();
    ClearBackground((Color){ 0, 32, 36, 255 });
    screen.Draw_Level(this->cam, false);
    BeginMode2D(cam->cam);

    for (int i = 0; i < objectManager.managed_objects.size(); ++i) {
        objectManager.managed_objects[i]->Draw();
    }

    screen.Draw_Level(this->cam, true);
}
