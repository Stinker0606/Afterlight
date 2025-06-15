#include "MenuScene.h"
#include <string>
#include <raylib.h>
#include "../scenes/Screen.h"
#include <Store.h>
#include "GameScene.h"
#include "../Vectors.h"

using namespace std::string_literals;

game::scenes::MenuScene::MenuScene()
{
    // Your scene initialization code here...
    //Screen screen;
    //screen.draw_Level();
}

game::scenes::MenuScene::~MenuScene()
{
    // Your scene cleanup code here...
}

void game::scenes::MenuScene::Update()
{
    // Your process input and update game scene code here...

    if (IsKeyPressed(KEY_ENTER))
        game::core::Store::stage->ReplaceWithNewScene("menu"s, "game"s, std::make_unique<GameScene>());
}

void game::scenes::MenuScene::Draw()
{
    /*// Your scene drawing code here...
    // Note that scene-actors are drawn automatically
    //DrawText("Menu - press ENTER to load game scene", 10, 10, 30, LIGHTGRAY);
    for (int i = 0; i < vec_cons.size(); ++i)
    {
        vec_cons[i].Draw();
    }
    for (int i = 0; i < vec_walls.size(); ++i)
    {
        vec_walls[i].Draw();
    }
    screen.Draw_Level();*/
}