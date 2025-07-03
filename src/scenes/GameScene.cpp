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
{dtm.Start();
    cam=std::make_shared<Cam>(this->mp);
    screen.Load_Tiled_Objects(objectManager,p_cm);

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
    mp.Tick(dtm.Get_Dt());

    this->cam->Cam_Movement(dtm.Get_Dt());
    dtm.Update();
}

void game::scenes::GameScene::Draw()
{
    BeginDrawing();
    ClearBackground(WHITE);
    screen.Draw_Level(this->cam);
    mp.Draw();



}