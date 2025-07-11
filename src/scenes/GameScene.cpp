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

using namespace std::string_literals;

game::scenes::GameScene::GameScene()
{
    dtm.Start();
    objectManager.AddObject(&mp);
    cam=std::make_shared<Cam>(this->mp);
    screen.LoadGameObjects(objectManager);

    // Your scene initialization code here...

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


    for (int i = 0; i < objectManager.managed_objects.size(); ++i) {
        objectManager.managed_objects[i]->Tick(dtm.Get_Dt());
    }
    this->cam->Cam_Movement(dtm.Get_Dt());
    this->p_cm->Check_Collisions();
    objectManager.Cleanup_Objects();
    dtm.Update();
}

void game::scenes::GameScene::Draw()
{
    BeginDrawing();
    ClearBackground(WHITE);
    screen.Draw_Level(this->cam, false);
    BeginMode2D(cam->cam);
    mp.Draw();
    objectManager.managed_objects[1]->Draw();

    screen.Draw_Level(this->cam, true);
}
