#include "PauseScene.h"
#include <string>
#include <raylib.h>
#include <Store.h>


using namespace std::string_literals;

game::scenes::PauseScene::PauseScene()
{
    // Your scene initialization code here...
}

game::scenes::PauseScene::~PauseScene()
{
    // Your scene cleanup code here...
}

void game::scenes::PauseScene::Update()
{
    // Your process input and update game scene code here...
    if (IsKeyPressed(KEY_ENTER))
        game::core::Store::stage->ReplaceWithExistingScene("pause"s, "game"s);
}

void game::scenes::PauseScene::Draw()
{
    // Your scene drawing code here...
    // Note that scene-actors are drawn automatically
    DrawText("Pause - press ENTER to re-enter the game scene", 10, 10, 30, LIGHTGRAY);
}
